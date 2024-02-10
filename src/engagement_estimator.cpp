#include "engagement_estimator.h"
#include "model_wrappers/features_extractor.h"
#include "model_wrappers/face_id.h"
#include "model_wrappers/emotion_classifier.h"
#include "model_wrappers/engagement_classifier.h"
#include "model_wrappers/mtcnn_wrapper.h"
#include "ui/screen_capture.h"

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
            double concat_double = 0;
            if (m_debugMod) {
                start = std::chrono::high_resolution_clock::now();
                startMeasure = std::chrono::high_resolution_clock::now();
            }
            QPixmap screenPixmap = ScreenCapture::capture(m_x, m_y, m_width, m_height);
            QPixmap downcasted = screenPixmap.scaledToWidth(m_detectorDownscaleWidth);
            double downcastRatioW = static_cast<double>(screenPixmap.width()) / downcasted.width();
            double downcastRatioH = static_cast<double>(screenPixmap.height()) / downcasted.height();
            qDebug() << "Downcast: " << downcastRatioW << "x" << downcastRatioH;
            auto faces = detector.detect(downcasted);
            //auto faces = detector.detect(screenPixmap);
            if (m_debugMod) {
                auto endMeasure = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> ms_double = endMeasure - startMeasure;
                resultInfo.inferTime["detector"] = ms_double.count();
            }
            std::chrono::time_point<std::chrono::high_resolution_clock> startLoop;
            if (m_debugMod) {
                startLoop = std::chrono::high_resolution_clock::now();
            }
            m_featuresExtractor.loadModel(extractor.getModelPath());
            m_faceIDExecutor.loadModel(faceId.getModelPath());
            for (auto& face : faces) {
                face.bbox.x1 *= downcastRatioW;
                face.bbox.x2 *= downcastRatioW;
                face.bbox.y1 *= downcastRatioH;
                face.bbox.y2 *= downcastRatioH;
                QPixmap f = screenPixmap.copy(face.bbox.x1, face.bbox.y1, face.bbox.x2 - face.bbox.x1, face.bbox.y2 - face.bbox.y1);
                if (m_debugMod) {
                    startMeasure = std::chrono::high_resolution_clock::now();
                }
                auto input = extractor.getInputTensor(f);
                auto faceEngEmoFeatures = extractor.getOutputTensor();
                m_featuresExtractor.run(input, faceEngEmoFeatures);
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
                m_faceIDExecutor.run(input, output);
                auto faceIdFeatures = faceId.getFaceIDFeatures(output);
                if (m_debugMod) {
                    auto endMeasure = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> ms_double = endMeasure - startMeasure;
                    faceid_double += ms_double.count();
                }
                resultInfo.faces.push_back({faceIdFeatures, faceEngEmoFeatures, f, face.bbox.x1, face.bbox.y1,
                                           face.bbox.x2, face.bbox.y2, timePoint});
                //qDebug() << "Result emotion: " << emotion.c_str() << ", text: ";
            }
            if (m_debugMod) {
                auto endMeasure = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> ms_double = endMeasure - startLoop;
                resultInfo.inferTime["extractorLoop"] = ms_double.count();
                startLoop = std::chrono::high_resolution_clock::now();
            }
            m_faceTracker.trackFaces(resultInfo);
            if (m_debugMod) {
                auto endMeasure = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> ms_double = endMeasure - startLoop;
                resultInfo.inferTime["tracker"] = ms_double.count();
                startLoop = std::chrono::high_resolution_clock::now();
            }
            m_emotionsExecutor.loadModel(emoClassifier.getModelPath());
            m_engagementExecutor.loadModel(engClassifier.getModelPath());
            for (auto& f : resultInfo.faces) {
                m_slidingWindow.AddFeatureVector(f.id, f.faceEngEmoFeatures);
                if (!m_slidingWindow.IsReady(f.id)) {
                    continue;
                }
                if (m_debugMod) {
                    startMeasure = std::chrono::high_resolution_clock::now();
                }
                auto inputSequence = m_slidingWindow.GetSequence(f.id);
                if (m_debugMod) {
                    auto endMeasure = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> ms_double = endMeasure - startMeasure;
                    concat_double += ms_double.count();
                }
                if (m_debugMod) {
                    startMeasure = std::chrono::high_resolution_clock::now();
                }
                auto output = emoClassifier.getOutputTensor();
                m_emotionsExecutor.run(inputSequence, output);
                f.emotionLabel = emoClassifier.classifyEmition(output).c_str();
                if (m_debugMod) {
                    auto endMeasure = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> ms_double = endMeasure - startMeasure;
                    emotions_double += ms_double.count();
                }
                if (m_debugMod) {
                    startMeasure = std::chrono::high_resolution_clock::now();
                }
                output = engClassifier.getOutputTensor();
                m_engagementExecutor.run(inputSequence, output);
                f.engagementLabel = engClassifier.classifyEngagement(output).c_str();
                if (m_debugMod) {
                    auto endMeasure = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> ms_double = endMeasure - startMeasure;
                    engagement_double += ms_double.count();
                }
            }
            if (m_debugMod) {
                auto endMeasure = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> ms_double = endMeasure - startLoop;
                resultInfo.inferTime["classifier"] = ms_double.count();
            }
            if (m_debugMod) {
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> ms_double = end - start;
                resultInfo.inferTime["all"] = ms_double.count();
                resultInfo.inferTime["features_extracton"] = features_extracton_double;
                resultInfo.inferTime["emotions"] = emotions_double;
                resultInfo.inferTime["engagement"] = engagement_double;
                resultInfo.inferTime["face_id"] = faceid_double;
                resultInfo.inferTime["aggregation"] = concat_double;
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
    m_frameInitialized = false;
    this->requestInterruption();
}
