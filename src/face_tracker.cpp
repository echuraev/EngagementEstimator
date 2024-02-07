#include "face_tracker.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <string>

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QString>

using MatF = std::vector<std::vector<float>>;
using MatI64 = std::vector<std::vector<int64_t>>;

namespace {
    struct FrameInfo {
        QPixmap frame;
        QString label;
    };

    MatF pairwiseDistances(const MatF& x, const MatF& y) {
        MatF res(x.size());
        for (size_t i = 0; i < x.size(); ++i) {
            res[i].resize(y.size());
            for (size_t j = 0; j < y.size(); ++j) {
                for (size_t k = 0; k < y[j].size(); ++k) {
                    res[i][j] += (x[i][k] - y[j][k]) * (x[i][k] - y[j][k]);
                }
                res[i][j] = std::sqrt(res[i][j]);
            }
        }
        return res;
    }

    MatI64 argsort(const MatF& array) {
        MatI64 indices(array.size());
        for (size_t i = 0; i < array.size(); ++i){
            indices[i].resize(array[i].size());
            std::iota(indices[i].begin(), indices[i].end(), 0);
            std::sort(indices[i].begin(), indices[i].end(),
                      [&array, &i](int left, int right) -> bool {
                          // sort indices according to corresponding array element
                          return array[i][left] < array[i][right];
                      });
        }

        return indices;
    }
}

FaceTracker::FaceTracker(const std::filesystem::path& outputDir)
    : m_outputDir(outputDir)
    , m_logFileName("engagement_estimator_log.csv")
{

}

void FaceTracker::trackFaces(ResultInfo& resultInfo)
{
    if (!openLogFile()) {
        qDebug() << "Cannot open " << m_logFileName;
    }
    MatF features;
    for (const auto& it : resultInfo.faces) {
        features.push_back(it.faceIdFeatures);
    }
    if (m_recentFeatures.empty()) {
        for (size_t i = 0; i < features.size(); ++i) {
            auto& face = resultInfo.faces[i];
            ++m_latestId;
            face.id = m_latestId;
            dumpInfo(face);
        }
        m_recentFeatures = features;
        return;
    }
    auto distMatrix = pairwiseDistances(features, m_recentFeatures);
    auto sortedIndices = argsort(distMatrix);

    for (size_t i = 0; i < sortedIndices.size(); ++i) {
        auto& face = resultInfo.faces[i];
        auto closestInd = sortedIndices[i][0];
        auto minDist = distMatrix[i][closestInd];
        if (minDist < 0.85 ||
                (sortedIndices[i].size() > 1 &&
                 minDist < distMatrix[i][sortedIndices[i][1]] - 0.1)) {
            m_recentFeatures[closestInd] = features[i];
            face.id = closestInd;
        } else {
            ++m_latestId;
            face.id = m_latestId;
            m_recentFeatures.push_back(features[i]);
        }
        //dumpInfo(face);
    }
    m_framesProcessed++;
}

bool FaceTracker::openLogFile() {
    auto absoluteFileName = m_outputDir / m_logFileName.toStdString();
    if (m_logFile.fileName() == absoluteFileName.c_str() && m_logFile.isOpen()) {
        return true;
    }
    if (m_logFile.isOpen())
        m_logFile.close();

    QString currentTime = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    m_outputDir /= currentTime.toStdString();
    QDir outDir(m_outputDir.c_str());
    if (!outDir.exists()) {
        if (!outDir.mkpath(m_outputDir.c_str()))
            return false;
    }

    auto filePath = m_outputDir / m_logFileName.toStdString();
    m_logFile.setFileName(filePath.c_str());
    if (!m_logFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    QTextStream out(&m_logFile);
    out << "Person ID;Image name;Emotion;Timestamp;x0;y0;x1;y1\n";
    return true;
}

void FaceTracker::dumpInfo(const FaceInfo& fi) {
    if (!m_logFile.isOpen())
        return;
    std::filesystem::path frameDirPath = std::to_string(m_framesProcessed);
    auto dumpDir = m_outputDir / frameDirPath;
    QDir qDumpDir;
    qDumpDir.mkpath(dumpDir.c_str());
    frameDirPath /= std::to_string(fi.id) + ".png";
    std::string relImgPath = frameDirPath.c_str();
    auto imgPath = m_outputDir / relImgPath;
    fi.frame.save(QString::fromStdString(imgPath.c_str()));
    QTextStream out(&m_logFile);
    out << fi.id << ";"
        << QString::fromStdString(relImgPath) << ";"
        //<< fi.label << ";"
        << fi.timestamp.time_since_epoch().count() << ";"
        << fi.x1 << ";"
        << fi.y1 << ";"
        << fi.x2 << ";"
        << fi.y2 << ";"
        << "\n";
}
