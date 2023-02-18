#include "faceid.h"

#include <QPixmap>

double l2_norm(const float* u, int n) {
    float accum = 0.f;
    for (int i = 0; i < n; ++i) {
        accum += u[i] * u[i];
    }
    return sqrt(accum);
}

FaceID::FaceID()
{

}

tvm::runtime::NDArray FaceID::getInputTensor(const QPixmap input)
{
    DLDevice devCPU{kDLCPU, 0};
    auto in_arr = tvm::runtime::NDArray::Empty({1, 3, 224, 224}, DLDataType{kDLFloat, 32, 1}, devCPU);
    QImage img = input.scaled(224, 224).toImage();
    img.convertTo(QImage::Format_RGB32);
    const float rMean = 0.485;
    const float gMean = 0.456;
    const float bMean = 0.406;
    const float rStd = 0.229;
    const float gStd = 0.224;
    const float bStd = 0.225;
    const int rChannelOffset = 0;
    const int gChannelOffset = img.height() * img.width();
    const int bChannelOffset = 2 * img.height() * img.width();
    for (int h = 0; h < img.height(); ++h) {
        for (int w = 0; w < img.width(); ++w) {
            QRgb rgb = img.pixel(w, h);
            float r = (static_cast<float>(qRed(rgb)) / 255 - rMean) / rStd;
            float g = (static_cast<float>(qGreen(rgb)) / 255 - gMean) / gStd;
            float b = (static_cast<float>(qBlue(rgb)) / 255 - bMean) / bStd;
            int offset = h * img.width() + w;
            int rIdx = rChannelOffset + offset;
            int gIdx = gChannelOffset + offset;
            int bIdx = bChannelOffset + offset;
            static_cast<float*>(in_arr->data)[rIdx] = r;
            static_cast<float*>(in_arr->data)[gIdx] = g;
            static_cast<float*>(in_arr->data)[bIdx] = b;
        }
    }
    return in_arr;
}

tvm::runtime::NDArray FaceID::getOutputTensor()
{
    DLDevice devCPU{kDLCPU, 0};
    return tvm::runtime::NDArray::Empty({1, 1280}, DLDataType{kDLFloat, 32, 1}, devCPU);
}

std::vector<float> FaceID::getFaceIDFeatures(const tvm::runtime::NDArray& output)
{
    const float* oData = static_cast<float*>(output->data);
    std::string out = "";
    auto l2 = l2_norm(oData, 1280);
    std::vector<float> data(1280);
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] = oData[i] / l2;
    }
    return data;
}

std::string FaceID::getModelPath()
{
    return "resources/models/state_vggface2_enet0_new.so";
}
