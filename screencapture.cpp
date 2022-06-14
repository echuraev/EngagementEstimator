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

void ScreenCapture::capture()
{
    qDebug() << "In capture:";
    QScreen *screen = QGuiApplication::primaryScreen();
    /*if (const QWindow *window = windowHandle())
        screen = window->screen();*/
    if (!screen) {
        qDebug() << " if (!screen) {";
        return;
    }
    auto originalPixmap = screen->grabWindow(0);
    originalPixmap.save("tmp.jpg");

    emit displayScreenshot(originalPixmap);
    /*QImage image = m_currentView->grabWindow();
    // QPixmap::grabWidget(m_currentView).save(path);
    image.save(path);*/
}
