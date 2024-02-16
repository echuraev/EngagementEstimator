#include "model_wrappers/model_wrapper_interface.h"

#include <QPixmap>

namespace ModelWrapperInterfaceCommon
{
std::vector<tvm::runtime::NDArray> prepareFacialImages(const std::vector<QPixmap>& inputs, const size_t batchSize) {
    DLDevice devCPU{kDLCPU, 0};
    std::vector<tvm::runtime::NDArray> res;
    size_t chunks = inputs.size() / batchSize;
    chunks += (inputs.size() % batchSize > 0) ? 1 : 0;
    for (size_t ch = 0; ch < chunks; ++ch) {
        auto in_arr = tvm::runtime::NDArray::Empty({static_cast<int64_t>(batchSize), 3, 224, 224}, DLDataType{kDLFloat, 32, 1}, devCPU);
        size_t imagesOffset = ch * batchSize;
        size_t imagesSize = std::min(batchSize, inputs.size() - imagesOffset);
        for (size_t i = 0; i < imagesSize; ++i) {
            QImage img = inputs[imagesOffset + i].scaled(224, 224).toImage();
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
            size_t imgOffset = i * img.height() * img.width() * 3;
            for (int h = 0; h < img.height(); ++h) {
                for (int w = 0; w < img.width(); ++w) {
                    QRgb rgb = img.pixel(w, h);
                    float r = (static_cast<float>(qRed(rgb)) / 255 - rMean) / rStd;
                    float g = (static_cast<float>(qGreen(rgb)) / 255 - gMean) / gStd;
                    float b = (static_cast<float>(qBlue(rgb)) / 255 - bMean) / bStd;
                    int offset = h * img.width() + w + imgOffset;
                    int rIdx = rChannelOffset + offset;
                    int gIdx = gChannelOffset + offset;
                    int bIdx = bChannelOffset + offset;
                    static_cast<float*>(in_arr->data)[rIdx] = r;
                    static_cast<float*>(in_arr->data)[gIdx] = g;
                    static_cast<float*>(in_arr->data)[bIdx] = b;
                }
            }
        }
        res.push_back(in_arr);
    }
    return res;
}

std::vector<std::vector<float>> divideBatchedFeaturesToVec(const std::vector<tvm::runtime::NDArray>& outputs, int64_t validOutputsNum) {
    std::vector<std::vector<float>> res;
    res.resize(validOutputsNum);
    for (size_t k = 0; k < outputs.size(); ++k) {
        const float* oData = static_cast<float*>(outputs[k]->data);
        size_t loopSize = (validOutputsNum - outputs[k].Shape()[0] > 0) ? outputs[k].Shape()[0] : validOutputsNum;
        validOutputsNum -= outputs[k].Shape()[0];
        for (size_t i = 0; i < loopSize; ++i) {
            std::vector<float>& data = res[k * outputs[k].Shape()[0] + i];
            data.insert(data.end(), &oData[i * outputs[k].Shape()[1]], &oData[(i + 1) * outputs[k].Shape()[1]]);
        }
    }
    return res;
}

std::vector<tvm::runtime::NDArray> divideBatchedFeaturesToTensors(const std::vector<tvm::runtime::NDArray>& outputs, int64_t validOutputsNum) {
    std::vector<tvm::runtime::NDArray> res;
    res.resize(validOutputsNum);
    for (size_t k = 0; k < outputs.size(); ++k) {
        const float* oData = static_cast<float*>(outputs[k]->data);
        size_t loopSize = (validOutputsNum - outputs[k].Shape()[0] > 0) ? outputs[k].Shape()[0] : validOutputsNum;
        validOutputsNum -= outputs[k].Shape()[0];
        for (size_t i = 0; i < loopSize; ++i) {
            res[k * outputs[k].Shape()[0] + i] = tvm::runtime::NDArray::Empty({1, outputs[k].Shape()[1]}, outputs[k]->dtype, outputs[k]->device);
            res[k * outputs[k].Shape()[0] + i].CopyFromBytes(&oData[i * outputs[k].Shape()[1]], outputs[k].Shape()[1] * sizeof(float));
        }
    }
    return res;
}
}
