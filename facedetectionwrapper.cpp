#include "facedetectionwrapper.h"

#include <QPixmap>

FaceDetectionWrapper::FaceDetectionWrapper()
{

}

tvm::runtime::NDArray FaceDetectionWrapper::getInputTensor(const QPixmap input)
{
    DLDevice devCPU{kDLCPU, 0};
    auto in_arr = tvm::runtime::NDArray::Empty({1, 3, 128, 128}, DLDataType{kDLFloat, 32, 1}, devCPU);
    QImage img = input.toImage();
    return in_arr;
}

tvm::runtime::NDArray FaceDetectionWrapper::getOutputBoxesTensor()
{
    DLDevice devCPU{kDLCPU, 0};
    return tvm::runtime::NDArray::Empty({1, 8}, DLDataType{kDLFloat, 32, 1}, devCPU);
}

std::string FaceDetectionWrapper::getModelPath()
{
    return "/Users/echuraev/Workspace/HSE/InvolvementEstimator/InvolvementEstimator/models/face_detection_short_range.so";
}
