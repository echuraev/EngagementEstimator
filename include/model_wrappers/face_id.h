#ifndef FACEID_H
#define FACEID_H

#include "model_wrappers/model_wrapper_interface.h"

#include <vector>

class QPixmap;

class FaceID : public ModelWrapperInterface
{
public:
    FaceID();
    std::vector<tvm::runtime::NDArray> getInputTensors(const std::vector<QPixmap>& inputs);
    tvm::runtime::NDArray getOutputTensor() final;
    void updateFaceIDFeatures(std::vector<std::vector<float>>& outputs);
    inline static size_t getBatchSize() { return m_batchSize; }

private:
    static const size_t m_batchSize = 10;
};

#endif // FACEID_H
