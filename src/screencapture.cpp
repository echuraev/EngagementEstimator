#include "screencapture.h"

#include <QQuickView>
#include <QGuiApplication>

#include <QDebug>

ScreenCapture::ScreenCapture()
{
}

QPixmap ScreenCapture::capture(int x, int y, int width, int height)
{
    static int counter = 0;
    counter++;
    QPoint point(x, y);
    QScreen *screen = QGuiApplication::screenAt(point);
    if (!screen) {
        throw std::runtime_error("Cannot get a QScreen for capturing");
    }
    auto p = screen->grabWindow(0, x, y, width, height);
    p.save("/Users/echuraev/screen.png");
    return p;
}
