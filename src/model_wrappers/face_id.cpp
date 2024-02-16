#include "model_wrappers/face_id.h"

#include <QPixmap>

double l2_norm(std::vector<float> vec) {
    float accum = 0.f;
    for (auto& it : vec) {
        accum += it * it;
    }
    return sqrt(accum);
}

FaceID::FaceID()
    : ModelWrapperInterface("resources/models/state_vggface2_enet0_new.so")
{

}

std::vector<tvm::runtime::NDArray> FaceID::getInputTensors(const std::vector<QPixmap>& inputs)
{
    return ModelWrapperInterfaceCommon::prepareFacialImages(inputs, m_batchSize);
}

tvm::runtime::NDArray FaceID::getOutputTensor()
{
    DLDevice devCPU{kDLCPU, 0};
    return tvm::runtime::NDArray::Empty({m_batchSize, 1280}, DLDataType{kDLFloat, 32, 1}, devCPU);
}

void FaceID::updateFaceIDFeatures(std::vector<std::vector<float>>& outputs)
{
    for (auto& out : outputs) {
        auto l2 = l2_norm(out);
        for (auto& it : out) {
            it /= l2;
        }
    }
}
