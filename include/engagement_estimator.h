#ifndef ENGAGEMENTESTIMATOR_H
#define ENGAGEMENTESTIMATOR_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <QPixmap>
#include <QThread>

#include "face_tracker.h"
#include "model_executor.h"
#include "result_info.h"
#include "sliding_window.h"

class EngagementEstimator : public QThread
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(bool running READ running);
    Q_PROPERTY(bool debugMod WRITE setDebugMod);
    Q_PROPERTY(QString outputDirectory WRITE setOutputDirectory);
public:
    explicit EngagementEstimator(QObject *parent = nullptr);
    void run();
    inline bool running() const {
        return m_running;
    }
    inline void setDebugMod(bool debugMod) {
        m_debugMod = debugMod;
    }
    void setOutputDirectory(const QString& outputDir);

public slots:
    void setFrameCoordinates(int x, int y, int width, int height);
    void stop();

signals:
    void error(const QString& msg);
    // Currently we use signal result only in debug mod.
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
    bool m_debugMod = false;

    ModelExecutor m_featuresExtractor;
    ModelExecutor m_faceIDExecutor;
    ModelExecutor m_emotionsExecutor;
    ModelExecutor m_engagementExecutor;

    SlidingWindow m_slidingWindow;

    const int m_detectorDownscaleWidth = 500;
};

#endif // ENGAGEMENTESTIMATOR_H
