#ifndef RESULTINFO_H
#define RESULTINFO_H

#include <QPixmap>
#include <QString>
#include <QVector>

struct FaceInfo {
    QString label;
    QString id;
    QPixmap frame;
    float x1;
    float y1;
    float x2;
    float y2;
};

struct ResultInfo {
    QVector<FaceInfo> faces;
#ifdef DEBUG_MOD
    double inferTime = 0;
#endif // DEBUG_MOD
};

#endif // RESULTINFO_H
