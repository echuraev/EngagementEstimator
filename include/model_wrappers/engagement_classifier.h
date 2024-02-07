#ifndef ENGAGEMENTCLASSIFIER_H
#define ENGAGEMENTCLASSIFIER_H

#include "model_wrappers/model_wrapper_interface.h"

class EngagementClassifier : public ModelWrapperInterface
{
public:
    EngagementClassifier();
    tvm::runtime::NDArray getOutputTensor() final;
    std::string classifyEngagement(const tvm::runtime::NDArray& output);
};

#endif // ENGAGEMENTCLASSIFIER_H
