#include "involvementestimator.h"
#include "modelexecutor.h"
#include "screencapture.h"

#include <QDebug>
#include <QGuiApplication>
#include <QPixmap>

InvolvementEstimator::InvolvementEstimator(QObject *parent)
    : QObject(parent)
{
}

void InvolvementEstimator::run(int x, int y, int width, int height)
{
    static int counter = 0;
    counter++;
    try {
        auto pixmap = ScreenCapture::capture(x, y, width, height);
        ModelExecutor executor;
        executor.loadModel("/Users/echuraev/Workspace/HSE/InvolvementEstimator/InvolvementEstimator/models/enet_b0_8_best_afew.so");
        QPixmap face("/Users/echuraev/Workspace/HSE/InvolvementEstimator/InvolvementEstimator/models/face_224_224.png");
        auto output = executor.run(face);
        qDebug() << "Result: " << output.c_str();
        /*QString number;
        number.setNum(counter);
        QString name = "/Users/echuraev/tmp/tmp" + number + ".jpg";
        pixmap.save(name);*/
    } catch (...) {
    }
}
