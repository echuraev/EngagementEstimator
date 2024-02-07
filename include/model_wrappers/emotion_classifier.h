#ifndef EMOTIONCLASSIFIER_H
#define EMOTIONCLASSIFIER_H

#include "model_wrappers/model_wrapper_interface.h"

class EmotionClassifier : public ModelWrapperInterface
{
public:
    EmotionClassifier();
    tvm::runtime::NDArray getOutputTensor() final;
    std::string classifyEmition(const tvm::runtime::NDArray& output);
};

#endif // EMOTIONCLASSIFIER_H
