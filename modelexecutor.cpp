#include "modelexecutor.h"

#include <QPixmap>
#include <QDebug>
#include <algorithm>

#undef slots
#include <tvm/runtime/module.h>
#include <tvm/runtime/packed_func.h>
#include <tvm/runtime/registry.h>
#define slots Q_SLOTS

ModelExecutor::ModelExecutor()
{
}

void ModelExecutor::loadModel(std::string modelName)
{
    // TODO: add check if file exists
    m_module = tvm::runtime::Module::LoadFromFile(modelName);
}

void ModelExecutor::run(const tvm::runtime::NDArray& input, tvm::runtime::NDArray& output, DeviceType devType)
{
    DLDevice dev;
    if (devType == DeviceType::CPU) {
        dev = {kDLCPU, 0};
    } else if (devType == DeviceType::Metal) {
        dev = {kDLMetal, 0};
    } else if (devType == DeviceType::OpenCL) {
        dev = {kDLOpenCL, 0};
    } else {
        throw std::runtime_error("Unexpected device type");
    }

    tvm::runtime::Module gmod = m_module.GetFunction("default")(dev);
    tvm::runtime::PackedFunc set_input = gmod.GetFunction("set_input");
    tvm::runtime::PackedFunc get_input = gmod.GetFunction("get_input");
    tvm::runtime::PackedFunc get_output = gmod.GetFunction("get_output");
    tvm::runtime::PackedFunc run = gmod.GetFunction("run");

    set_input(0, input);
    TVMSynchronize(dev.device_type, dev.device_id, nullptr);

    run();
    TVMSynchronize(dev.device_type, dev.device_id, nullptr);

    // output
    tvm::runtime::NDArray y = get_output(0);
    output.CopyFrom(y);
    TVMSynchronize(dev.device_type, dev.device_id, nullptr);
}
