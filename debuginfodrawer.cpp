#include "debuginfodrawer.h"
#include <QDebug>

DebugInfoDrawer::DebugInfoDrawer()
{

}

void DebugInfoDrawer::paint(QPainter *painter)
{
    painter->save();
    const int fontSize = 20;
    painter->setPen(QPen(Qt::red, 3));
    QFont font = painter->font();
    font.setPointSize(fontSize);
    painter->setFont(font);

    for (auto& f : m_debugInfo.faces) {
        QList<QLineF> lines;
        lines.append({f.x1, f.y1, f.x2, f.y1});
        lines.append({f.x2, f.y1, f.x2, f.y2});
        lines.append({f.x2, f.y2, f.x1, f.y2});
        lines.append({f.x1, f.y2, f.x1, f.y1});
        painter->drawLines(lines);
        QString text = "Emotion: " + f.label + ", id: " + f.id;
        painter->drawText(QPointF(f.x1, f.y1 - 5), text);
    }
    if (m_debugInfo.inferTime > 0) {
        QString text = "Infer time: " + QString::number(m_debugInfo.inferTime) + " ms.";
        painter->drawText(QPointF(10, 20), text);
    }

    painter->restore();
}

void DebugInfoDrawer::recvResults(const DebugInfo& debugInfo)
{
    m_debugInfo = debugInfo;
    update();
}
