#include "engagement_estimator.h"
#include "model_wrappers/features_extractor.h"
#include "model_wrappers/face_id.h"
#include "model_wrappers/emotion_classifier.h"
#include "model_wrappers/engagement_classifier.h"
#include "model_wrappers/mtcnn_wrapper.h"
#include "ui/screen_capture.h"

#include "ui/debug_info_drawer.h"

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
    FeaturesExtractor extractor;
    FaceID faceId;
    EmotionClassifier emoClassifier;
    EngagementClassifier engClassifier;
    MTCNNWrapper detector;

    while (!this->isInterruptionRequested()) {
        try {
            auto timePoint = std::chrono::system_clock::now();
            ResultInfo resultInfo;
            std::chrono::time_point<std::chrono::high_resolution_clock> start, startMeasure;
            double features_extracton_double = 0;
            double faceid_double = 0;
            double emotions_double = 0;
            double engagement_double = 0;
            if (m_debugMod) {
                start = std::chrono::high_resolution_clock::now();
            }
            auto screenPixmap = ScreenCapture::capture(m_x, m_y, m_width, m_height);
            //qDebug() << "screen pixmap: " << screenPixmap;
            /*QFile file("/Users/echuraev/test_screenpixmap.png");
            file.open(QIODevice::WriteOnly);
            screenPixmap.save(&file, "PNG");*/
            const int coef = std::max(screenPixmap.width() / m_width, screenPixmap.height() / m_height);
            qDebug() << "Here 112342";
            if (m_debugMod) {
                startMeasure = std::chrono::high_resolution_clock::now();
            }
            auto faces = detector.detect(screenPixmap);
            if (m_debugMod) {
                auto endMeasure = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> ms_double = endMeasure - startMeasure;
                resultInfo.inferTime["detector"] = ms_double.count();
            }

            featuresExtractor.loadModel(extractor.getModelPath());
            faceIDExecutor.loadModel(faceId.getModelPath());
            for (auto& face : faces) {
                QPixmap f = screenPixmap.copy(face.bbox.x1, face.bbox.y1, face.bbox.x2 - face.bbox.x1, face.bbox.y2 - face.bbox.y1);
                if (m_debugMod) {
                    startMeasure = std::chrono::high_resolution_clock::now();
                }
                auto input = extractor.getInputTensor(f);
                auto faceEngEmoFeatures = extractor.getOutputTensor();
                featuresExtractor.run(input, faceEngEmoFeatures);
                if (m_debugMod) {
                    auto endMeasure = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> ms_double = endMeasure - startMeasure;
                    features_extracton_double += ms_double.count();
                }
                if (m_debugMod) {
                    startMeasure = std::chrono::high_resolution_clock::now();
                }
                input = faceId.getInputTensor(f);
                auto output = faceId.getOutputTensor();
                faceIDExecutor.run(input, output);
                auto faceIdFeatures = faceId.getFaceIDFeatures(output);
                if (m_debugMod) {
                    auto endMeasure = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> ms_double = endMeasure - startMeasure;
                    faceid_double += ms_double.count();
                }
                resultInfo.faces.push_back({faceIdFeatures, faceEngEmoFeatures, f, face.bbox.x1/coef, face.bbox.y1/coef,
                                           face.bbox.x2/coef, face.bbox.y2/coef, timePoint});
                //qDebug() << "Result emotion: " << emotion.c_str() << ", text: ";
            }
            m_faceTracker.trackFaces(resultInfo);
            emotionsExecutor.loadModel(emoClassifier.getModelPath());
            engagementExecutor.loadModel(engClassifier.getModelPath());
            for (auto& f : resultInfo.faces) {
                if (m_debugMod) {
                    startMeasure = std::chrono::high_resolution_clock::now();
                }
                if (m_debugMod) {
                    auto endMeasure = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> ms_double = endMeasure - startMeasure;
                    emotions_double += ms_double.count();
                }
                if (m_debugMod) {
                    startMeasure = std::chrono::high_resolution_clock::now();
                }
                if (m_debugMod) {
                    auto endMeasure = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> ms_double = endMeasure - startMeasure;
                    engagement_double += ms_double.count();
                }
            }
            if (m_debugMod) {
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> ms_double = end - start;
                resultInfo.inferTime["all"] = ms_double.count();
                resultInfo.inferTime["features_extracton"] = features_extracton_double;
                resultInfo.inferTime["emotions"] = emotions_double;
                resultInfo.inferTime["engagement"] = engagement_double;
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