#include "engagementestimator.h"
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
#include <QMutex>
#include <QPixmap>

EngagementEstimator::EngagementEstimator(QObject *parent)
    : QThread(parent)
    //, m_faceTracker("./")
    , m_faceTracker(std::filesystem::current_path() / "test_output")
{
}

void EngagementEstimator::run()
{
    if (!m_frameInitialized) {
        emit error("Error! Frame coordinates should be initialized.");
        return;
    }
    m_running = true;
    while (!this->isInterruptionRequested()) {
        try {
            auto timePoint = std::chrono::system_clock::now();
            ResultInfo resultInfo;
#ifdef DEBUG_MOD
            auto start = std::chrono::high_resolution_clock::now();
#endif
            MTCNNWrapper detector;
            //OCRWrapper ocr;
            auto screenPixmap = ScreenCapture::capture(m_x, m_y, m_width, m_height);
            const int coef = std::max(screenPixmap.width() / m_width, screenPixmap.height() / m_height);
#ifdef DEBUG_MOD
            auto startMeasure = std::chrono::high_resolution_clock::now();
#endif
            auto faces = detector.detect(screenPixmap);
#ifdef DEBUG_MOD
            auto endMeasure = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> ms_double = endMeasure - startMeasure;
            resultInfo.inferTime["detector"] = ms_double.count();
            double emotions_double = 0;
            double faceid_double = 0;
#endif
            //ocr.Init();

            ModelExecutor executor, faceIDExecutor;
            executor.loadModel(EnetB08BestAfewWrapper::getModelPath());
            faceIDExecutor.loadModel(FaceID::getModelPath());
            for (auto& face : faces) {
                QPixmap f = screenPixmap.copy(face.bbox.x1, face.bbox.y1, face.bbox.x2 - face.bbox.x1, face.bbox.y2 - face.bbox.y1);
#ifdef DEBUG_MOD
                startMeasure = std::chrono::high_resolution_clock::now();
#endif
                auto input = EnetB08BestAfewWrapper::getInputTensor(f);
                auto output = EnetB08BestAfewWrapper::getOutputTensor();
                executor.run(input, output);
                auto emotion = EnetB08BestAfewWrapper::classifyEmition(output);
#ifdef DEBUG_MOD
                endMeasure = std::chrono::high_resolution_clock::now();
                ms_double = endMeasure - startMeasure;
                emotions_double += ms_double.count();
#endif
                //auto id = ocr.getTextFromImg(f);
#ifdef DEBUG_MOD
                startMeasure = std::chrono::high_resolution_clock::now();
#endif
                input = FaceID::getInputTensor(f);
                output = FaceID::getOutputTensor();
                faceIDExecutor.run(input, output);
                auto visualEmbeddings = FaceID::getFaceIDFeatures(output);
#ifdef DEBUG_MOD
                endMeasure = std::chrono::high_resolution_clock::now();
                ms_double = endMeasure - startMeasure;
                faceid_double += ms_double.count();
#endif
                resultInfo.faces.push_back({emotion.c_str(), visualEmbeddings, f, face.bbox.x1/coef, face.bbox.y1/coef,
                                           face.bbox.x2/coef, face.bbox.y2/coef, timePoint});
                qDebug() << "Result emotion: " << emotion.c_str() << ", text: ";
            }
            m_faceTracker.trackFaces(resultInfo);
#ifdef DEBUG_MOD
            auto end = std::chrono::high_resolution_clock::now();
            ms_double = end - start;
            resultInfo.inferTime["all"] = ms_double.count();
            resultInfo.inferTime["emotions"] = emotions_double;
            resultInfo.inferTime["face_id"] = faceid_double;
            emit result(resultInfo);
#endif
            qDebug() << "Result size: " << faces.size();
        } catch (std::exception& e) {
            emit error(e.what());
            break;
        } catch (...) {
            emit error("Error! Something went wrong.");
            break;
        }
    }
    m_running = false;
}

void EngagementEstimator::setFrameCoordinates(int x, int y, int width, int height) {
    static QMutex lock;
    QMutexLocker locker(&lock);
    m_frameInitialized = true;
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
}

void EngagementEstimator::stop() {
    this->requestInterruption();
}
