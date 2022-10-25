#include "ocrwrapper.h"

#include <tesseract/baseapi.h>

#include <QDebug>

OCRWrapper::OCRWrapper()
{
}

OCRWrapper::~OCRWrapper()
{
    if (m_api.get() != nullptr) {
        m_api->End();
    }
}

void OCRWrapper::Init()
{
    if (m_initialized)
        return;
    m_api = std::make_unique<tesseract::TessBaseAPI>();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (m_api->Init(NULL, "eng")) {
        throw std::runtime_error("Cannot initialize tesseract!");
    }
    m_initialized = true;
}

QString OCRWrapper::getTextFromImg(const QPixmap& img) const
{
    if (!m_initialized)
        throw std::runtime_error("OCRWrapper should be initialized before using!");
    QImage qImg = img.toImage();
    int bytesPerPixel = qImg.depth() / 8; // bites per pixel to bytes per pixel
    m_api->SetImage(qImg.bits(), qImg.width(), qImg.height(), bytesPerPixel, qImg.bytesPerLine());
    char* outText = m_api->GetUTF8Text();
    // TODO: Do something with text.
    // https://github.com/livezingy/tesstsv2map
    // char* outText = m_api->GetTSVText(0);
    QString res(outText);
    delete [] outText;
    return res;
}
