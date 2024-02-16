#ifndef MODEL_WRAPPER_INTERFACE_H
#define MODEL_WRAPPER_INTERFACE_H

#undef slots
#include <tvm/runtime/module.h>
#define slots Q_SLOTS

#include <vector>

class QPixmap;

namespace ModelWrapperInterfaceCommon
{
    std::vector<tvm::runtime::NDArray> prepareFacialImages(const std::vector<QPixmap>& inputs, size_t batchSize);
    std::vector<std::vector<float>> divideBatchedFeaturesToVec(const std::vector<tvm::runtime::NDArray>& outputs, int64_t validOutputsNum);
    std::vector<tvm::runtime::NDArray> divideBatchedFeaturesToTensors(const std::vector<tvm::runtime::NDArray>& outputs, int64_t validOutputsNum);
}

class ModelWrapperInterface
{
public:
    ModelWrapperInterface(const std::string& modelPath) : m_modelPath(modelPath) {}
    virtual tvm::runtime::NDArray getOutputTensor() = 0;
    inline std::string getModelPath() { return m_modelPath; }
protected:
    std::string m_modelPath;
};

#endif // MODEL_WRAPPER_INTERFACE_H
