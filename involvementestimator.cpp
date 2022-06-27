#include "involvementestimator.h"
#include "screencapture.h"

#include <QDebug>
#include <QGuiApplication>
#include <QPixmap>


/*#include <tvm/runtime/module.h>
#include <tvm/runtime/packed_func.h>
#include <tvm/runtime/registry.h>*/

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
        QString number;
        number.setNum(counter);
        QString name = "/Users/echuraev/tmp/tmp" + number + ".jpg";
        pixmap.save(name);
    } catch (...) {
    }
}
