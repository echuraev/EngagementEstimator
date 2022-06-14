#ifndef SCREENCAPTURE_H
#define SCREENCAPTURE_H

#include <QObject>
#include <QPixmap>
//#include <QWidget>

/*class QString;
class QQuickView;
*/
class ScreenCapture : public QObject
{
    Q_OBJECT
public:
    explicit ScreenCapture(QObject *parent = nullptr);

public slots:
    void capture();

signals:
    void displayScreenshot(const QPixmap& img);


private:
    QPixmap originalPixmap;
    //QQuickView *m_currentView;
};

#endif // SCREENCAPTURE_H
