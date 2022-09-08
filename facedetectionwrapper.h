#ifndef FACEDETECTIONWRAPPER_H
#define FACEDETECTIONWRAPPER_H

class QPixmap;

#undef slots
#include <tvm/runtime/module.h>
#define slots Q_SLOTS

class FaceDetectionWrapper
{
public:
    FaceDetectionWrapper();
    static tvm::runtime::NDArray getInputTensor(const QPixmap input);
    static tvm::runtime::NDArray getOutputBoxesTensor();
    static tvm::runtime::NDArray getOutputScoresTensor();
    //static std::string classifyEmition(const tvm::runtime::NDArray& output);
    static std::string getModelPath();
};

#endif // FACEDETECTIONWRAPPER_H
