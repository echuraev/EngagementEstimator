import torch
import tvm
import tvm.testing
from tvm import relay
from tvm.contrib import utils, xcode, coreml_runtime, graph_runtime
from tvm import auto_scheduler
from tvm.relay import transform
from tvm.relay.build_module import bind_params_by_name

target = "llvm -mtriple=x86_64-apple-darwin20.1.0"
model_name = "enet_b0_8_best_afew"
model = model_name + ".pt"
extractor_log_file = "logs/enet_b0_8_best_afew.auto-scheduler.log"
SAMPLES = 120

def open_pytorch_model(path):
    torch.set_grad_enabled(False)

    model = torch.load(path, map_location=torch.device('cpu'))
    model.eval()
    #last_layer=torch.nn.Sequential(model.classifier[0],torch.nn.Softmax(dim=1))
    #inp = torch.randn(20, 1280).to(device)
    #f=last_layer.forward(inp)
    ##print(f.shape,f,f.sum(axis=1))
    model.classifier = torch.nn.Identity()
    input_shape = [1, 3, 224, 224]
    inp = torch.rand(input_shape)

    with torch.no_grad():
        trace = torch.jit.trace(model, inp)

    input_name = "input0"
    shape_list = [(input_name, inp.shape)]

    mod, params = relay.frontend.from_pytorch(trace, shape_list)
    return mod, params


def single_attention_model(N_CLASSES, weights, SAMPLES=None):
    from tensorflow.keras import backend as K
    from tensorflow.keras.optimizers import Adam, SGD
    from tensorflow.keras.models import Model, Sequential, load_model, model_from_json
    from tensorflow.keras.layers import Input, Dropout, Dense, Activation, Concatenate, Reshape
    from tensorflow.keras.layers import Flatten, RepeatVector, Permute, TimeDistributed
    from tensorflow.keras.layers import Multiply, Lambda, Softmax
    from tensorflow.keras.saving import save_model

    FEATURE_VECTOR_DIM = 2560
    inputs = Input(shape=(SAMPLES, FEATURE_VECTOR_DIM),name='image_set')  # (batch, samples, features)
    e = Dense(1, activation='linear', name='e')(inputs)
    #e = Reshape([-1], name='alignment')(e)
    e = Reshape([SAMPLES], name='alignment')(e)
    alpha = Activation('softmax', name='alpha')(e)

    alpha_repeated = Permute([2, 1],name='alpha_repeated')(RepeatVector(FEATURE_VECTOR_DIM, name='repeat')(alpha))

    c = Multiply(name='c')([inputs, alpha_repeated])
    x = Lambda(lambda xin: K.sum(xin, axis=1), output_shape=(FEATURE_VECTOR_DIM,), name='context')(c)

    #x = Dropout(0.3)(x)
    x = Dense(units=512, activation='relu', name='hidden_FC')(x)  # (batch, units) #128 64
    #x = tf.keras.activations.gelu(Dense(512, activation='linear')(x))

    pred=Dense(N_CLASSES, activation='softmax')(x)  # (batch, classes)
    modelAtn = Model(inputs=inputs, outputs=pred)

    modelAtn.load_weights(weights)
    modelAtn.summary()

    return modelAtn, {"image_set": [1, SAMPLES, FEATURE_VECTOR_DIM]}


def open_keras_model(model, shape_dict):
    mod, params = relay.frontend.from_keras(model, shape_dict, layout="NHWC")
    return mod, params


def build_model(mod, params, log_file=None):
    if log_file is not None:
        with auto_scheduler.ApplyHistoryBest(log_file):
            with tvm.transform.PassContext(opt_level=3, config={"relay.backend.use_auto_scheduler": True}):
                lib = relay.build(mod, target=target, params=params)
    else:
        with tvm.transform.PassContext(opt_level=3):
            lib = relay.build(mod, target=target, params=params)

    return lib


if __name__ == '__main__':
    # Extractor model
    mod, params = open_pytorch_model(model)
    lib = build_model(mod, params, extractor_log_file)
    lib_name = "enet_b0_8_best_afew_extractor.so"
    lib.export_library(lib_name)

    # Emotions single attention classifier
    modelAtn, shape_dict = single_attention_model(8, "./weights/enet_b0_8_best_afew.pt_universal_single_attention_max_weights.h5", SAMPLES)
    mod, params = open_keras_model(modelAtn, shape_dict)
    lib = build_model(mod, params)
    lib_name = "single_attention_emotions_classifier.so"
    lib.export_library(lib_name)

    # Engagement single attention classifier
    modelAtn, shape_dict = single_attention_model(2, "./weights/enet_b0_8_best_afew.pt_single_attention_independent_bin_class_best.h5", SAMPLES)
    mod, params = open_keras_model(modelAtn, shape_dict)
    lib = build_model(mod, params)
    lib_name = "single_attention_emotions_classifier.so"
    lib.export_library(lib_name)
