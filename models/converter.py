import torch
import tvm
import tvm.testing
from tvm import relay
from tvm.contrib import utils, xcode, coreml_runtime, graph_runtime


from tvm.relay import transform
from tvm.relay.build_module import bind_params_by_name

target = "llvm -mtriple=x86_64-apple-darwin20.1.0"
model_name = "enet_b0_8_best_afew"
model = model_name + ".pt"

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

    convert_to_relay = True

    if convert_to_relay:
        with torch.no_grad():
            trace = torch.jit.trace(model, inp)

        input_name = "input0"
        shape_list = [(input_name, inp.shape)]

        mod, params = relay.frontend.from_pytorch(trace, shape_list)

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
    lib = open_pytorch_model(model)
    #lib_name = model_name + ".so"
    #path_dso = lib_name
    #lib.export_library(path_dso)



