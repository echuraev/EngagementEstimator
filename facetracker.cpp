#include "facetracker.h"

#include <chrono>
#include <string>

#include <QDebug>
#include <QString>

namespace {
    struct FrameInfo {
        QPixmap frame;
        QString label;
    };
}

FaceTracker::FaceTracker()
{

}

void FaceTracker::trackFaces(uint64_t timestamp, const ResultInfo& resultInfo)
{
    std::unordered_map<std::string, FrameInfo> resultMap;
    QVector<TrackerInfo> finalizeTrackingVec;
    for (const auto& ri : resultInfo.faces) {
        resultMap[ri.id.toStdString()] = {ri.frame, ri.label};
    }
    QVector<std::string> toRemove;
    for (auto& [id, info] : m_trackingFaces) {
        auto search = resultMap.find(id);
        if (search != resultMap.end()) {
            // Emotion didn't change
            if (info.label == search->second.label) {
                info.frames.push_back(search->second.frame);
            } else {
                // new emotion
                finalizeTrackingVec.push_back(info);
                info = {{search->second.frame}, search->second.label, id.c_str(), timestamp};
            }
            resultMap.erase(id);
        } else {
            finalizeTrackingVec.push_back(info);
            toRemove.push_back(id);
        }
    }
    for (auto& it : toRemove) {
        m_trackingFaces.erase(it);
    }
    for (auto& [id, info] : resultMap) {
        m_trackingFaces[id] = {{info.frame}, info.label, id.c_str(), timestamp};
    }
    finalizeTracking(timestamp, finalizeTrackingVec);
}

void FaceTracker::finalizeTracking(uint64_t timestamp, const QVector<TrackerInfo>& vec) const
{
    for (auto& it : vec) {
        std::chrono::milliseconds duration(timestamp - it.timestamp);
        qDebug() << "Finalize: " << timestamp << ", id: " << it.id << ", label: " << it.label
                 << ", duration: " << duration.count() << " ms, frames: " << it.frames.size();
    }
}
