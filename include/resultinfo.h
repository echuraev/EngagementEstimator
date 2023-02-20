#ifndef RESULTINFO_H
#define RESULTINFO_H

#include <QPixmap>
#include <QString>
#include <QVector>

#include <unordered_map>

using Timestamp = std::chrono::time_point<std::chrono::system_clock>;

struct FaceInfo {
    QString label;
    std::vector<float> embeddings;
    QPixmap frame;
    float x1;
    float y1;
    float x2;
    float y2;
    Timestamp timestamp;
    int64_t id;
};

struct ResultInfo {
    QVector<FaceInfo> faces;
#ifdef DEBUG_MOD
    std::unordered_map<std::string, double> inferTime = {};
#endif // DEBUG_MOD
};

#endif // RESULTINFO_H
