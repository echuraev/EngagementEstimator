#include "enetb08bestafewwrapper.h"
#include "mtcnnwrapper.h"
#include "involvementestimator.h"
#include "modelexecutor.h"
#include "screencapture.h"
#include "ocrwrapper.h"

#ifdef DEBUG_MOD
#include "debuginfodrawer.h"

#include <chrono>
#endif

#include <string>

#include <QDebug>
#include <QGuiApplication>
#include <QPixmap>

InvolvementEstimator::InvolvementEstimator(QObject *parent)
    : QObject(parent)
{
}

void InvolvementEstimator::run(int x, int y, int width, int height)
{
    try {
#ifdef DEBUG_MOD
        DebugInfo debugInfo;
        auto start = std::chrono::high_resolution_clock::now();
#endif
        auto screenPixmap = ScreenCapture::capture(x, y, width, height);
        const int coef = std::max(screenPixmap.width() / width, screenPixmap.height() / height);
        MTCNNWrapper detector;
        auto faces = detector.detect(screenPixmap);
        OCRWrapper ocr;
        ocr.Init();

        ModelExecutor executor;
        executor.loadModel(EnetB08BestAfewWrapper::getModelPath());
        for (auto& face : faces) {
            QPixmap f = screenPixmap.copy(face.bbox.x1, face.bbox.y1, face.bbox.x2 - face.bbox.x1, face.bbox.y2 - face.bbox.y1);
            auto input = EnetB08BestAfewWrapper::getInputTensor(f);
            auto output = EnetB08BestAfewWrapper::getOutputTensor();
            executor.run(input, output);
            auto emotion = EnetB08BestAfewWrapper::classifyEmition(output);
            auto id = ocr.getTextFromImg(f);
#ifdef DEBUG_MOD
            debugInfo.faces.push_back({emotion.c_str(), id, face.bbox.x1/coef, face.bbox.y1/coef,
                                       face.bbox.x2/coef, face.bbox.y2/coef});
#endif
            qDebug() << "Result emotion: " << emotion.c_str() << ", text: " << id;
        }
#ifdef DEBUG_MOD
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> ms_double = end - start;
        debugInfo.inferTime = ms_double.count();
        emit resultDebug(debugInfo);
#endif
        qDebug() << "Result size: " << faces.size();
    } catch (std::exception& e) {
        emit error(e.what());
    } catch (...) {
        emit error("Error! Something went wrong.");
    }
}
