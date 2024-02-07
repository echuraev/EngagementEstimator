#include "model_wrappers/features_extractor.h"

#include <QPixmap>

FeaturesExtractor::FeaturesExtractor()
    : ModelWrapperInterface("resources/models/enet_b0_8_best_afew_extractor.so")
{

}

tvm::runtime::NDArray FeaturesExtractor::getInputTensor(const QPixmap input)
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

tvm::runtime::NDArray FeaturesExtractor::getOutputTensor()
{
    DLDevice devCPU{kDLCPU, 0};
    return tvm::runtime::NDArray::Empty({1, 1280}, DLDataType{kDLFloat, 32, 1}, devCPU);
}
