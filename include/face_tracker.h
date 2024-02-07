#ifndef FACETRACKER_H
#define FACETRACKER_H

#include <QFile>
#include <QPixmap>
#include <QVector>

#include <chrono>
#include <filesystem>
#include <string>
#include <unordered_map>

#include "result_info.h"

class FaceTracker
{
public:
    FaceTracker(const std::filesystem::path& outputDir);
    void trackFaces(ResultInfo& resultInfo);
    inline void setLogFileName(const QString& logFileName) { m_logFileName = logFileName; }
    inline void setOutputDir(const std::filesystem::path& outputDir) { m_outputDir = outputDir; }

private:
    bool openLogFile();
    void dumpInfo(const FaceInfo& fi);

private:
    std::vector<std::vector<float>> m_recentFeatures = {};
    std::filesystem::path m_outputDir;
    int64_t m_latestId = -1;
    uint64_t m_framesProcessed = 0;
    QString m_logFileName;
    QFile m_logFile;
};

#endif // FACETRACKER_H
