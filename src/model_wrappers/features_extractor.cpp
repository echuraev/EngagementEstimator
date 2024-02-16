#include "model_wrappers/features_extractor.h"

#include <QPixmap>

FeaturesExtractor::FeaturesExtractor()
    : ModelWrapperInterface("resources/models/enet_b0_8_best_afew_extractor.so")
{

}

std::vector<tvm::runtime::NDArray> FeaturesExtractor::getInputTensors(const std::vector<QPixmap>& inputs)
{
    return ModelWrapperInterfaceCommon::prepareFacialImages(inputs, m_batchSize);
}

tvm::runtime::NDArray FeaturesExtractor::getOutputTensor()
{
    DLDevice devCPU{kDLCPU, 0};
    return tvm::runtime::NDArray::Empty({m_batchSize, 1280}, DLDataType{kDLFloat, 32, 1}, devCPU);
}
