#include "model_wrappers/engagement_classifier.h"

EngagementClassifier::EngagementClassifier()
    : ModelWrapperInterface("resources/models/single_attention_engagement_classifier.so")
{

}

tvm::runtime::NDArray EngagementClassifier::getOutputTensor()
{
    DLDevice devCPU{kDLCPU, 0};
    return tvm::runtime::NDArray::Empty({1, 2}, DLDataType{kDLFloat, 32, 1}, devCPU);
}

std::string EngagementClassifier::classifyEngagement(const tvm::runtime::NDArray& output)
{
    const std::vector<std::string> engagement = {"Distracted", "Engaged"};
    const float* oData = static_cast<float*>(output->data);
    size_t maxIdx = std::distance(oData, std::max_element(oData, oData + 8));

    return engagement[maxIdx];
}
