#ifndef DEBUGINFODRAWER_H
#define DEBUGINFODRAWER_H

#include <QPainter>
#include <QQuickPaintedItem>

#include "resultinfo.h"

class DebugInfoDrawer : public QQuickPaintedItem
{
    Q_OBJECT
public:
    DebugInfoDrawer();
    ~DebugInfoDrawer() = default;

    void paint(QPainter *painter) final;

#ifdef DEBUG_MOD
public slots:
    void recvResults(const ResultInfo& resultInfo);

private:
    ResultInfo m_resultInfo;
#endif // DEBUG_MOD
};

#endif // DEBUGINFODRAWER_H
