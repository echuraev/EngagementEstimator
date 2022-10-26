#include "ocrwrapper.h"

#include <tesseract/baseapi.h>

#include <QDebug>

#include <map>

// Take implementation from here: https://github.com/livezingy/tesstsv2map
namespace {
std::vector<std::string> splitString(const std::string &str,const char* delimiter)
{
    std::vector<std::string> splited;
    std::string s(str);
    size_t pos;

    while ((pos = s.find(delimiter)) != std::string::npos)
    {
        std::string sec = s.substr(0, pos);

        if (!sec.empty())
        {
            splited.push_back(s.substr(0, pos));
        }

        s = s.substr(pos + 1);
    }

    splited.push_back(s);

    return splited;
}

/*
Output:
std::map<std::string, std::vector<std::string>> result
results["level"][i]     get level
results["block_num"][i]  get block_num
results["page_num"][i]  get page_num
results["par_num"][i]   get par_num
results["line_num"][i]  get line_num
results["word_num"][i]  get word_num
results["left"][i], results["top"][i], results["width"][i], results["height" ][i],the coordinates of the upper left corner, width and height.
results["conf"][i] to get the confidence of the OCR word.
results["text"][i] to get the OCR word.
*/
std::map<std::string, std::vector<std::string>> imageToData(const std::string& tsvText) {
    std::map<std::string, std::vector<std::string>> result;
    const std::vector<std::string> headVec = {"level", "page_num", "block_num", "par_num", "line_num",
                                              "word_num", "left", "top", "width", "height", "conf", "text"};
    //split the content by the "\n"
    auto tsvVec = splitString(tsvText, "\n");
    if (tsvVec.size() <= 1)
        return result;
    for (size_t i = 0; i < tsvVec.size() - 1; ++i) {
        auto lineVec = splitString(tsvVec[i], "\t");
        for (size_t j = 0; j < lineVec.size(); j++) {
            result[headVec[j]].push_back(lineVec[j]);
        }
        if (lineVec.size() < headVec.size()) {
            result[headVec[headVec.size() - 1]].push_back("");
        }
    }
    return result;
}
}

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

    /*char* outText = m_api->GetTSVText(0);
    auto tcvMap = imageToData(outText);
    //std::vector<QString> frame_text;
    QString res = "";
    for (size_t i = 0; i < tcvMap["text"].size(); ++i) {
        // x = results["left"][i]
        // y = results["top"][i]
        // w = results["width"][i]
        // h = results["height"][i]
        // text = results["text"][i].strip()
        // conf = float(results["conf"][i])
        // if conf > 0 and len(text)>1:
        //     frame_text.append((text,int(x/frame.shape[1]*frame_bgr.shape[1]),int(y/frame.shape[0]*frame_bgr.shape[0]),
        //                      int(w/frame.shape[1]*frame_bgr.shape[1]),int(h/frame.shape[0]*frame_bgr.shape[1])))
        double conf = QString(tcvMap["conf"][i].c_str()).toDouble();
        auto text = tcvMap["text"][i];
        if (conf > 0 && text.length() > 1) {
            res += text.c_str();
        }
    }*/
    char* outText = m_api->GetUTF8Text();
    QString res(outText);
    delete [] outText;
    return res;
}
