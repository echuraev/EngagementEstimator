#ifndef FACETRACKER_H
#define FACETRACKER_H

#include <QPixmap>
#include <QVector>

#include <chrono>
#include <string>
#include <unordered_map>

#include "resultinfo.h"

class FaceTracker
{
public:
    using Timestamp = std::chrono::time_point<std::chrono::system_clock>;
    FaceTracker();
    void trackFaces(Timestamp timestamp, const ResultInfo& resultInfo);

private:
    struct TrackerInfo {
        QVector<QPixmap> frames;
        QString label;
        QString id;
        Timestamp timestamp;
    };
    void finalizeTracking(Timestamp timestamp, const QVector<TrackerInfo>& vec) const;

private:
    std::unordered_map<std::string, TrackerInfo> m_trackingFaces = {};
};

#endif // FACETRACKER_H
