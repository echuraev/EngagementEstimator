#include "involvementestimator.h"
#include "enetb08bestafewwrapper.h"
#include "faceid.h"
#include "mtcnnwrapper.h"
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
    //, m_faceTracker("./")
    , m_faceTracker(std::filesystem::current_path() / "test_output")
{
}

void InvolvementEstimator::run(int x, int y, int width, int height)
{
    try {
        auto timePoint = std::chrono::system_clock::now();
        ResultInfo resultInfo;
#ifdef DEBUG_MOD
        auto start = std::chrono::high_resolution_clock::now();
#endif
        MTCNNWrapper detector;
        //OCRWrapper ocr;
        auto screenPixmap = ScreenCapture::capture(x, y, width, height);
        const int coef = std::max(screenPixmap.width() / width, screenPixmap.height() / height);
        auto faces = detector.detect(screenPixmap);
        //ocr.Init();

        ModelExecutor executor, faceIDExecutor;
        executor.loadModel(EnetB08BestAfewWrapper::getModelPath());
        faceIDExecutor.loadModel(FaceID::getModelPath());
        for (auto& face : faces) {
            QPixmap f = screenPixmap.copy(face.bbox.x1, face.bbox.y1, face.bbox.x2 - face.bbox.x1, face.bbox.y2 - face.bbox.y1);
            auto input = EnetB08BestAfewWrapper::getInputTensor(f);
            auto output = EnetB08BestAfewWrapper::getOutputTensor();
            executor.run(input, output);
            auto emotion = EnetB08BestAfewWrapper::classifyEmition(output);
            //auto id = ocr.getTextFromImg(f);
            input = FaceID::getInputTensor(f);
            output = FaceID::getOutputTensor();
            faceIDExecutor.run(input, output);
            auto visualEmbeddings = FaceID::getFaceIDFeatures(output);
            resultInfo.faces.push_back({emotion.c_str(), visualEmbeddings, f, face.bbox.x1/coef, face.bbox.y1/coef,
                                       face.bbox.x2/coef, face.bbox.y2/coef});
            qDebug() << "Result emotion: " << emotion.c_str() << ", text: ";
        }
        m_faceTracker.trackFaces(timePoint, resultInfo);
#ifdef DEBUG_MOD
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> ms_double = end - start;
        resultInfo.inferTime = ms_double.count();
        emit result(resultInfo);
#endif
        qDebug() << "Result size: " << faces.size();
    } catch (std::exception& e) {
        emit error(e.what());
    } catch (...) {
        emit error("Error! Something went wrong.");
    }
}
