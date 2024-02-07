#ifndef SCREENCAPTURE_H
#define SCREENCAPTURE_H

#include <QPixmap>

class ScreenCapture
{
public:
    ScreenCapture();
    static QPixmap capture(int x, int y, int width, int height);
};

#endif // SCREENCAPTURE_H
