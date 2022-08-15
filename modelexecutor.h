#ifndef MODELEXECUTOR_H
#define MODELEXECUTOR_H

#include <string>

class QPixmap;

#undef slots
#include <tvm/runtime/module.h>
#define slots Q_SLOTS

enum class DeviceType {
    CPU,
    OpenCL,
    Metal
};

class ModelExecutor
{
public:
    ModelExecutor();
    void loadModel(std::string modelName);
    //void setInput(QPixmap pixmap);
    //void run();
    //void getOutput();
    std::string run(QPixmap input, DeviceType devType = DeviceType::CPU);
private:
    tvm::runtime::NDArray pyTorchPreprocess(QPixmap input);
private:
    tvm::runtime::Module m_module;
};

#endif // MODELEXECUTOR_H
