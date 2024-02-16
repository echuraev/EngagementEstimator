import torch
import timm
import tvm
import tvm.testing
from tvm import relay
from tvm.contrib import utils, xcode, coreml_runtime, graph_runtime


from tvm.relay import transform
from tvm.relay.build_module import bind_params_by_name

target = "llvm -mtriple=x86_64-apple-darwin20.1.0"
model_name = "state_vggface2_enet0_new"
BATCH = 10


def open_faceid_model():
    torch.set_grad_enabled(False)

    model = timm.create_model('tf_efficientnet_b0_ns', pretrained=False)
    model.classifier = torch.nn.Identity()
    model.load_state_dict(torch.load('state_vggface2_enet0_new.pt', map_location=torch.device('cpu')))
    device = 'cpu'


    model = model.to(device)
    print(model.eval())
    model.eval()
    input_shape = [BATCH, 3, 224, 224]
    inp = torch.rand(input_shape)

    convert_to_relay = True

    if convert_to_relay:
        with torch.no_grad():
            trace = torch.jit.trace(model, inp)

        input_name = "input0"
        shape_list = [(input_name, inp.shape)]

        mod, params = relay.frontend.from_pytorch(trace, shape_list)
        print('-' * 50)
        print(trace)
        print('-' * 50)

    with tvm.transform.PassContext(opt_level=3):
        if params:
            mod["main"] = bind_params_by_name(mod["main"], params)

        seq = tvm.transform.Sequential(
            [
                transform.InferType(),
                transform.FoldConstant(),
            ]
        )

        mod = seq(mod)
        print('*' * 50)
        print(mod)
        print('*' * 50)
        lib = relay.build(mod, target=target, params=params)

    return lib

if __name__ == '__main__':
    lib = open_faceid_model()
    lib_name = model_name + ".so"
    path_dso = lib_name
    lib.export_library(path_dso)



