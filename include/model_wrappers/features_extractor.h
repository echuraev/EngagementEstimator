#ifndef FEATURESEXTRACTOR_H
#define FEATURESEXTRACTOR_H

#include "model_wrappers/model_wrapper_interface.h"

class QPixmap;

class FeaturesExtractor : public ModelWrapperInterface
{
public:
    FeaturesExtractor();
    tvm::runtime::NDArray getInputTensor(const QPixmap input);
    tvm::runtime::NDArray getOutputTensor() final;
};

#endif // FEATURESEXTRACTOR_H
