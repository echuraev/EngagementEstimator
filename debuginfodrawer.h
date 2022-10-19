#ifndef DEBUGINFODRAWER_H
#define DEBUGINFODRAWER_H

#include <QPainter>
#include <QQuickPaintedItem>
#include <QVector>

#include "involvementestimator.h"

class DebugInfoDrawer : public QQuickPaintedItem
{
    Q_OBJECT
public:
    DebugInfoDrawer();
    ~DebugInfoDrawer() = default;
    void paint(QPainter *painter) final;

public slots:
    void recvResults(const QVector<FaceInfo>& faces);

private:
    QVector<FaceInfo> m_faces;
};

#endif // DEBUGINFODRAWER_H
