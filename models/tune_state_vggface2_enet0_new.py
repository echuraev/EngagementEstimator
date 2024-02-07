import torch
import timm
import tvm
import tvm.testing
from tvm import auto_scheduler
from tvm import relay
from tvm.contrib import utils, xcode, coreml_runtime, graph_runtime


from tvm.relay import transform
from tvm.relay.build_module import bind_params_by_name

target = "llvm -mtriple=x86_64-apple-darwin22.1.0"
model_name = "state_vggface2_enet0_new"
log_file = "{}.auto-scheduler.log".format(model_name)
model = model_name + ".pt"

def open_faceid_model():
    torch.set_grad_enabled(False)

    model = timm.create_model('tf_efficientnet_b0_ns', pretrained=False)
    model.classifier = torch.nn.Identity()
    model.load_state_dict(torch.load('state_vggface2_enet0_new.pt', map_location=torch.device('cpu')))
    device = 'cpu'


    model = model.to(device)
    print(model.eval())
    model.eval()
    input_shape = [1, 3, 224, 224]
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
    return mod, params

def tune_model(mod, params, target, n_trials, log_file):
    # extract workloads from relay program
    def extract_tasks(mod, target, params):
        print("Mod:")
        print(mod)
        print("Extract tasks...")
        tasks, task_weights = auto_scheduler.extract_tasks(mod, params, target)
        assert(len(tasks) > 0)

        for idx, task in enumerate(tasks):
            print("Task: %d, desc: %s" % (idx, task.desc))
        return tasks, task_weights

    tasks, task_weights = extract_tasks(mod, target, params)
    print("Begin tuning...")
    tuner = auto_scheduler.TaskScheduler(tasks, task_weights, load_log_file=log_file)
    tune_option = auto_scheduler.TuningOptions(
        num_measure_trials=n_trials,  # change this to 20000 to achieve the best performance
        runner=auto_scheduler.LocalRunner(repeat=10, enable_cpu_cache_flush=True),
        measure_callbacks=[auto_scheduler.RecordToFile(log_file)],
    )

    tuner.tune(tune_option)

def build_model(mod, params, log_file):
    with auto_scheduler.ApplyHistoryBest(log_file):
        with tvm.transform.PassContext(opt_level=3, config={"relay.backend.use_auto_scheduler": True}):
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
    mod, params = open_faceid_model()
    #tune_model(mod, params, target, 10000, log_file)
    lib = build_model(mod, params, log_file)
    lib_name = model_name + ".tuned.so"
    lib.export_library(lib_name)
