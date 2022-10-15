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
        for (auto& face : faces) {
            QPixmap f = screenPixmap.copy(face.bbox.x1, face.bbox.y1, face.bbox.x2 - face.bbox.x1, face.bbox.y2 - face.bbox.y1);
            auto input = EnetB08BestAfewWrapper::getInputTensor(f);
            auto output = EnetB08BestAfewWrapper::getOutputTensor();
            executor.run(input, output);
            auto emotion = EnetB08BestAfewWrapper::classifyEmition(output);
            qDebug() << "Result: " << emotion.c_str();
        }
          qDebug() << "Result: " << faces.size();
    } catch (...) {
        emit error("Error! Something went wrong.");
    }
}
