#ifndef FEATURESEXTRACTOR_H
#define FEATURESEXTRACTOR_H

#include "model_wrappers/model_wrapper_interface.h"

class QPixmap;

class FeaturesExtractor : public ModelWrapperInterface
{
public:
    FeaturesExtractor();
    std::vector<tvm::runtime::NDArray> getInputTensors(const std::vector<QPixmap>& inputs);
    tvm::runtime::NDArray getOutputTensor() final;
    inline static size_t getBatchSize() { return m_batchSize; }

private:
    static const size_t m_batchSize = 10;
};

#endif // FEATURESEXTRACTOR_H
