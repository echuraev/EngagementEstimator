import tvm
from tvm.contrib import graph_executor
import numpy as np

LIB_FILE = "enet_b0_8_best_afew.tuned.so"
#LIB_FILE = "state_vggface2_enet0_new.tuned.so"

def evaluate_performance(lib, input_name, data_shape, dtype="float32"):
    # upload parameters to device
    dev = tvm.cpu()
    data_tvm = tvm.nd.array((np.random.uniform(size=data_shape)).astype(dtype))
    module = graph_executor.GraphModule(lib["default"](dev))
    module.set_input(input_name, data_tvm)

    # evaluate
    print("Evaluate inference time cost...")
    print(module.benchmark(dev, number=10, repeat=100))

def read_lib(lib_file):
    return tvm.runtime.load_module(lib_file)

if __name__ == '__main__':
    lib = read_lib(LIB_FILE)
    input_name = "input0"
    input_shape = (1, 3, 224, 224)
    evaluate_performance(lib, input_name, input_shape)
