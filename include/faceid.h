#ifndef FACEID_H
#define FACEID_H

#include <vector>
#undef slots
#include <tvm/runtime/module.h>
#define slots Q_SLOTS

class QPixmap;

class FaceID
{
public:
    FaceID();
    static tvm::runtime::NDArray getInputTensor(const QPixmap input);
    static tvm::runtime::NDArray getOutputTensor();
    static std::vector<float> getFaceIDFeatures(const tvm::runtime::NDArray& output);
    static std::string getModelPath();
};

#endif // FACEID_H
