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

    for (auto& f : m_faces) {
        QRectF rect(f.x1, f.y1, f.x2 - f.x1, f.y2 - f.y1);
        QList<QLineF> lines;
        lines.append({f.x1, f.y1, f.x2, f.y1});
        lines.append({f.x2, f.y1, f.x2, f.y2});
        lines.append({f.x2, f.y2, f.x1, f.y2});
        lines.append({f.x1, f.y2, f.x1, f.y1});
        painter->drawLines(lines);
        painter->drawText(QPointF(f.x1, f.y1 - 5), f.m_label);
    }

    painter->restore();
}

void DebugInfoDrawer::recvResults(const QVector<FaceInfo>& faces)
{
    m_faces = faces;
    update();
}
