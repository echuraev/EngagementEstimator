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
        const int coef = std::max(screenPixmap.width() / width, screenPixmap.height() / height);
        MTCNNWrapper detector;
        auto faces = detector.detect(screenPixmap);

        ModelExecutor executor;
        executor.loadModel(EnetB08BestAfewWrapper::getModelPath());
        //#ifdef DEBUG_MOD
        QVector<FaceInfo> faceInfo;
        //#endif
        for (auto& face : faces) {
            QPixmap f = screenPixmap.copy(face.bbox.x1, face.bbox.y1, face.bbox.x2 - face.bbox.x1, face.bbox.y2 - face.bbox.y1);
            auto input = EnetB08BestAfewWrapper::getInputTensor(f);
            auto output = EnetB08BestAfewWrapper::getOutputTensor();
            executor.run(input, output);
            auto emotion = EnetB08BestAfewWrapper::classifyEmition(output);
            faceInfo.push_back({emotion.c_str(), face.bbox.x1/coef, face.bbox.y1/coef, face.bbox.x2/coef, face.bbox.y2/coef});
            qDebug() << "Result: " << emotion.c_str();
        }
        emit resultDebug(faceInfo);
          qDebug() << "Result: " << faces.size();
    } catch (std::exception& e) {
        emit error(e.what());
    } catch (...) {
        emit error("Error! Something went wrong.");
    }
}
