#include "mtcnnwrapper.h"

#include <QPixmap>

MTCNNWrapper::MTCNNWrapper()
    : m_detector(m_pConfig, m_rConfig, m_oConfig)
{
}

std::vector<Face> MTCNNWrapper::detect(const QPixmap& input)
{
    QImage qImg = input.toImage();
    qImg.convertTo(QImage::Format_RGB888);
    cv::Mat img(qImg.height(), qImg.width(), CV_8UC3, (cv::Scalar*)qImg.rgbSwapped().scanLine(0));
    auto faces = m_detector.detect(img, 20.f, 0.709f);
    return faces;
}
