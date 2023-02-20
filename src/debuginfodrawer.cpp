#include "debuginfodrawer.h"
#include "engagementestimator.h"
#include <QDebug>

DebugInfoDrawer::DebugInfoDrawer()
{

}

#ifdef DEBUG_MOD
void DebugInfoDrawer::paint(QPainter *painter)
{
    painter->save();
    const int fontSize = 20;
    painter->setPen(QPen(Qt::red, 3));
    QFont font = painter->font();
    font.setPointSize(fontSize);
    painter->setFont(font);

    for (auto& f : m_resultInfo.faces) {
        QList<QLineF> lines;
        lines.append({f.x1, f.y1, f.x2, f.y1});
        lines.append({f.x2, f.y1, f.x2, f.y2});
        lines.append({f.x2, f.y2, f.x1, f.y2});
        lines.append({f.x1, f.y2, f.x1, f.y1});
        painter->drawLines(lines);
        QString text = "Emotion: " + f.label + ", id: " + QString::number(f.id);
        painter->drawText(QPointF(f.x1, f.y1 - 5), text);
    }
    if (!m_resultInfo.inferTime.empty()) {
        int xPos = 10;
        int yPos = 20;
        QString text = "Infer time: ";
        for (auto& [key, val] : m_resultInfo.inferTime) {
            text += QString::fromStdString(key) + ": " + QString::number(val) + " ms. ";
        }
        painter->drawText(QPointF(xPos, yPos), text);
    }

    painter->restore();
}

void DebugInfoDrawer::recvResults(const ResultInfo& resultInfo)
{
    m_resultInfo = resultInfo;
    update();
}
#else
void DebugInfoDrawer::paint(QPainter *painter)
{
}
#endif // DEBUG_MOD
