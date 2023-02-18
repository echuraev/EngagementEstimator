#ifndef ENGAGEMENTESTIMATOR_H
#define ENGAGEMENTESTIMATOR_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <QPixmap>
#include <QThread>

#include "resultinfo.h"
#include "facetracker.h"

class EngagementEstimator : public QThread
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(bool running READ running);
public:
    explicit EngagementEstimator(QObject *parent = nullptr);
    void run();
    inline bool running() const {
        return m_running;
    }

public slots:
    void setFrameCoordinates(int x, int y, int width, int height);
    void stop();

signals:
    void error(const QString& msg);
    // Currently we use signal result only in DEBUG_MOD.
    // Probably something will be changed in the future
    void result(const ResultInfo& resultInfo);

private:
    bool run(int x, int y, int width, int height);

private:
    FaceTracker m_faceTracker;
    bool m_frameInitialized = false;
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    bool m_running = false;
};

#endif // ENGAGEMENTESTIMATOR_H
