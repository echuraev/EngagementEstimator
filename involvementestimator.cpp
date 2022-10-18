#include "enetb08bestafewwrapper.h"
#include "mtcnnwrapper.h"
#include "involvementestimator.h"
#include "modelexecutor.h"
#include "screencapture.h"

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
        auto screenPixmap = ScreenCapture::capture(x, y, width, height);
        MTCNNWrapper detector;
        auto faces = detector.detect(screenPixmap);

        ModelExecutor executor;
        executor.loadModel(EnetB08BestAfewWrapper::getModelPath());
        //#ifdef DEBUG_MOD
        QVector<QString> label;
        QVector<float> x1, y1, x2, y2;
        //#endif
        for (auto& face : faces) {
            QPixmap f = screenPixmap.copy(face.bbox.x1, face.bbox.y1, face.bbox.x2 - face.bbox.x1, face.bbox.y2 - face.bbox.y1);
            auto input = EnetB08BestAfewWrapper::getInputTensor(f);
            auto output = EnetB08BestAfewWrapper::getOutputTensor();
            executor.run(input, output);
            auto emotion = EnetB08BestAfewWrapper::classifyEmition(output);
            label.push_back(emotion.c_str());
            x1.push_back(face.bbox.x1);
            y1.push_back(face.bbox.y1);
            x2.push_back(face.bbox.x2);
            y2.push_back(face.bbox.y2);
            qDebug() << "Result: " << emotion.c_str();
        }
        emit resultDebug(label, x1, y1, x2, y2);
          qDebug() << "Result: " << faces.size();
    } catch (std::exception& e) {
        emit error(e.what());
    } catch (...) {
        emit error("Error! Something went wrong.");
    }
}
