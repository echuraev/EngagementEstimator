#include "screencapture.h"

#include <QPixmap>
#include <QQuickView>
#include <QString>
#include <QGuiApplication>

#include <QDebug>

ScreenCapture::ScreenCapture(QObject *parent)
    : QObject(parent)
{
}

void ScreenCapture::capture(int x, int y, int width, int height, int frameRate)
{
    qDebug() << "In capture: x: " << x << ", y: " << y << ", width: " << width << ", height: " << height;
    QPoint point(x, y);
    QScreen *screen = QGuiApplication::screenAt(point);
    if (!screen) {
        qDebug() << " if (!screen) {";
        return;
    }
    auto originalPixmap = screen->grabWindow(0, x, y, width, height);
    originalPixmap.save("/Users/echuraev/tmp.jpg");
}
