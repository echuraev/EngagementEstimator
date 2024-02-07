#ifndef FACEID_H
#define FACEID_H

#include "model_wrappers/model_wrapper_interface.h"

#include <vector>

class QPixmap;

class FaceID : public ModelWrapperInterface
{
public:
    FaceID();
    tvm::runtime::NDArray getInputTensor(const QPixmap input);
    tvm::runtime::NDArray getOutputTensor() final;
    std::vector<float> getFaceIDFeatures(const tvm::runtime::NDArray& output);
};

#endif // FACEID_H
