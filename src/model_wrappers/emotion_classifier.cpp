#include "model_wrappers/emotion_classifier.h"

EmotionClassifier::EmotionClassifier()
    : ModelWrapperInterface("resources/models/single_attention_emotions_classifier.so")
{

}

tvm::runtime::NDArray EmotionClassifier::getOutputTensor()
{
    DLDevice devCPU{kDLCPU, 0};
    return tvm::runtime::NDArray::Empty({1, 8}, DLDataType{kDLFloat, 32, 1}, devCPU);
}

std::string EmotionClassifier::classifyEmition(const tvm::runtime::NDArray& output)
{
    const std::vector<std::string> emotions = {"Anger", "Contempt", "Disgust", "Fear", "Happiness", "Neutral", "Sadness", "Surprise"};
    const float* oData = static_cast<float*>(output->data);
    size_t maxIdx = std::distance(oData, std::max_element(oData, oData + 8));

    return emotions[maxIdx];
}
