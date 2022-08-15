#include "modelexecutor.h"

#include <QPixmap>
#include <QDebug>
#include <algorithm>

#undef slots
#include <tvm/runtime/module.h>
#include <tvm/runtime/packed_func.h>
#include <tvm/runtime/registry.h>
#define slots Q_SLOTS

namespace {
    const std::vector<std::string> emotions = {"Anger", "Contempt", "Disgust", "Fear", "Happiness", "Neutral", "Sadness", "Surprise"};
}

ModelExecutor::ModelExecutor()
{
}

void ModelExecutor::loadModel(std::string modelName)
{
    // TODO: add check if file exists
    m_module = tvm::runtime::Module::LoadFromFile(modelName);
}

std::string ModelExecutor::run(QPixmap input, DeviceType devType)
{
    DLDevice dev;
    if (devType == DeviceType::CPU) {
        dev = {kDLCPU, 0};
    } else if (devType == DeviceType::Metal) {
        dev = {kDLMetal, 0};
    } else if (devType == DeviceType::OpenCL) {
        dev = {kDLOpenCL, 0};
    } else {
        throw std::runtime_error("Unexpected device type");
    }
    DLDevice devCPU{kDLCPU, 0};

    tvm::runtime::Module gmod = m_module.GetFunction("default")(dev);
    tvm::runtime::PackedFunc set_input = gmod.GetFunction("set_input");
    tvm::runtime::PackedFunc get_input = gmod.GetFunction("get_input");
    tvm::runtime::PackedFunc get_output = gmod.GetFunction("get_output");
    tvm::runtime::PackedFunc run = gmod.GetFunction("run");

    auto in_arr = pyTorchPreprocess(input);
    set_input(0, in_arr);
    TVMSynchronize(dev.device_type, dev.device_id, nullptr);


    run();
    TVMSynchronize(dev.device_type, dev.device_id, nullptr);

    // output
    auto output = tvm::runtime::NDArray::Empty({1, 8}, DLDataType{kDLFloat, 32, 1}, devCPU);
    tvm::runtime::NDArray y = get_output(0);
    output.CopyFrom(y);
    TVMSynchronize(dev.device_type, dev.device_id, nullptr);

    const float* oData = static_cast<float*>(output->data);
    size_t maxIdx = std::distance(oData, std::max_element(oData, oData + 8));

    return emotions[maxIdx];
}

tvm::runtime::NDArray ModelExecutor::pyTorchPreprocess(QPixmap input)
{
    DLDevice devCPU{kDLCPU, 0};
    auto in_arr = tvm::runtime::NDArray::Empty({1, 3, 224, 224}, DLDataType{kDLFloat, 32, 1}, devCPU);
    QImage img = input.toImage();
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
            //qDebug() << "r: " << r << ", " << static_cast<float*>(in_arr->data)[rIdx];
        }
    }
    return in_arr;
}
