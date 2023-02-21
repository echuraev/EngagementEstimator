#include "engagementestimator.h"
#include "enetb08bestafewwrapper.h"
#include "faceid.h"
#include "mtcnnwrapper.h"
#include "modelexecutor.h"
#include "screencapture.h"
#include "ocrwrapper.h"

#include "debuginfodrawer.h"

#include <chrono>

#include <string>

#include <QDebug>
#include <QGuiApplication>
#include <QMutex>
#include <QPixmap>

EngagementEstimator::EngagementEstimator(QObject *parent)
    : QThread(parent)
    , m_faceTracker(std::filesystem::current_path() / "test_output")
{
}

void EngagementEstimator::setOutputDirectory(const QString& outputDir) {
    if (outputDir.size() == 0) {
        qDebug() << "Warning! The output directory wasn't passed to the app. Using current direcotry.";
        return;
    }
    m_faceTracker.setOutputDir(outputDir.toStdString());
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
            std::chrono::time_point<std::chrono::high_resolution_clock> start, startMeasure;
            double emotions_double = 0;
            double faceid_double = 0;
            if (m_debugMod) {
                start = std::chrono::high_resolution_clock::now();
            }
            MTCNNWrapper detector;
            //OCRWrapper ocr;
            auto screenPixmap = ScreenCapture::capture(m_x, m_y, m_width, m_height);
            const int coef = std::max(screenPixmap.width() / m_width, screenPixmap.height() / m_height);
            if (m_debugMod) {
                startMeasure = std::chrono::high_resolution_clock::now();
            }
            auto faces = detector.detect(screenPixmap);
            if (m_debugMod) {
                auto endMeasure = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> ms_double = endMeasure - startMeasure;
                resultInfo.inferTime["detector"] = ms_double.count();
            }
            //ocr.Init();

            ModelExecutor executor, faceIDExecutor;
            executor.loadModel(EnetB08BestAfewWrapper::getModelPath());
            faceIDExecutor.loadModel(FaceID::getModelPath());
            for (auto& face : faces) {
                QPixmap f = screenPixmap.copy(face.bbox.x1, face.bbox.y1, face.bbox.x2 - face.bbox.x1, face.bbox.y2 - face.bbox.y1);
                if (m_debugMod) {
                    startMeasure = std::chrono::high_resolution_clock::now();
                }
                auto input = EnetB08BestAfewWrapper::getInputTensor(f);
                auto output = EnetB08BestAfewWrapper::getOutputTensor();
                executor.run(input, output);
                auto emotion = EnetB08BestAfewWrapper::classifyEmition(output);
                if (m_debugMod) {
                    auto endMeasure = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> ms_double = endMeasure - startMeasure;
                    emotions_double += ms_double.count();
                }
                //auto id = ocr.getTextFromImg(f);
                if (m_debugMod) {
                    startMeasure = std::chrono::high_resolution_clock::now();
                }
                input = FaceID::getInputTensor(f);
                output = FaceID::getOutputTensor();
                faceIDExecutor.run(input, output);
                auto visualEmbeddings = FaceID::getFaceIDFeatures(output);
                if (m_debugMod) {
                    auto endMeasure = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> ms_double = endMeasure - startMeasure;
                    faceid_double += ms_double.count();
                }
                resultInfo.faces.push_back({emotion.c_str(), visualEmbeddings, f, face.bbox.x1/coef, face.bbox.y1/coef,
                                           face.bbox.x2/coef, face.bbox.y2/coef, timePoint});
                qDebug() << "Result emotion: " << emotion.c_str() << ", text: ";
            }
            m_faceTracker.trackFaces(resultInfo);
            if (m_debugMod) {
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> ms_double = end - start;
                resultInfo.inferTime["all"] = ms_double.count();
                resultInfo.inferTime["emotions"] = emotions_double;
                resultInfo.inferTime["face_id"] = faceid_double;
                emit result(resultInfo);
            }
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
