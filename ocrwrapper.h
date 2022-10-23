#ifndef OCRWRAPPER_H
#define OCRWRAPPER_H

#include <QPixmap>
#include <QString>

#include <memory>

namespace tesseract {
    class TessBaseAPI;
}

class OCRWrapper
{
public:
    OCRWrapper();
    ~OCRWrapper();
    void Init();
    QString getTextFromImg(const QPixmap& img) const;

private:
    std::unique_ptr<tesseract::TessBaseAPI> m_api;
    bool m_initialized = false;
};

#endif // OCRWRAPPER_H
