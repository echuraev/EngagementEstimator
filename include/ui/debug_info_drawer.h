#ifndef DEBUGINFODRAWER_H
#define DEBUGINFODRAWER_H

#include <QPainter>
#include <QQuickPaintedItem>

#include "result_info.h"

class DebugInfoDrawer : public QQuickPaintedItem
{
    Q_OBJECT
public:
    DebugInfoDrawer();
    ~DebugInfoDrawer() = default;

    void paint(QPainter *painter) final;

public slots:
    void recvResults(const ResultInfo& resultInfo);

private:
    ResultInfo m_resultInfo;
    uint borderMargin = 20;
};

#endif // DEBUGINFODRAWER_H
