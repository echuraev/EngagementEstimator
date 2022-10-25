#ifndef FACETRACKER_H
#define FACETRACKER_H

#include <QPixmap>
#include <QVector>

#include <string>
#include <unordered_map>

#include "resultinfo.h"

class FaceTracker
{
public:
    FaceTracker();
    void trackFaces(uint64_t timestamp, const ResultInfo& resultInfo);

private:
    struct TrackerInfo {
        QVector<QPixmap> frames;
        QString label;
        QString id;
        uint64_t timestamp;
    };
    void finalizeTracking(uint64_t timestamp, const QVector<TrackerInfo>& vec) const;

private:
    std::unordered_map<std::string, TrackerInfo> m_trackingFaces = {};
};

#endif // FACETRACKER_H
