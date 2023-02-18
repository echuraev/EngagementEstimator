#ifndef FACETRACKER_H
#define FACETRACKER_H

#include <QFile>
#include <QPixmap>
#include <QVector>

#include <chrono>
#include <filesystem>
#include <string>
#include <unordered_map>

#include "resultinfo.h"

class FaceTracker
{
public:
    using Timestamp = std::chrono::time_point<std::chrono::system_clock>;
    FaceTracker(const std::filesystem::path& outputDir);
    void trackFaces(Timestamp timestamp, ResultInfo& resultInfo);
    inline void setLogFileName(const QString& logFileName) { m_logFileName = logFileName; }
    //inline void setOutputDir(const QString& outputDir) { m_outputDir = outputDir; }

private:
    struct DumperInfo {
        QPixmap frame;
        QString label;
        int64_t id;
        Timestamp timestamp;
    };
    bool openLogFile();
    void dumpInfo(const DumperInfo& di);

private:
    std::vector<std::vector<float>> m_recentFeatures = {};
    std::filesystem::path m_outputDir;
    int64_t m_latestId = -1;
    uint64_t m_framesProcessed = 0;
    QString m_logFileName;
    QFile m_logFile;
};

#endif // FACETRACKER_H
