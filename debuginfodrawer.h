#ifndef DEBUGINFODRAWER_H
#define DEBUGINFODRAWER_H

#include <QPainter>
#include <QQuickPaintedItem>
#include <QVector>

struct FaceInfo {
    QString label;
    QString id;
    float x1;
    float y1;
    float x2;
    float y2;
};

struct DebugInfo {
    QVector<FaceInfo> faces;
    double inferTime = 0;
};

class DebugInfoDrawer : public QQuickPaintedItem
{
    Q_OBJECT
public:
    DebugInfoDrawer();
    ~DebugInfoDrawer() = default;
    void paint(QPainter *painter) final;

public slots:
    void recvResults(const DebugInfo& debugInfo);

private:
    DebugInfo m_debugInfo;
};

#endif // DEBUGINFODRAWER_H
