#ifndef INVOLVEMENTESTIMATOR_H
#define INVOLVEMENTESTIMATOR_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <QPixmap>
//#ifdef DEBUG_MOD
#include <QVector>

struct FaceInfo {
    QString m_label;
    float x1;
    float y1;
    float x2;
    float y2;
};

//#endif

class InvolvementEstimator : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit InvolvementEstimator(QObject *parent = nullptr);

public slots:
    void run(int x, int y, int width, int height);

signals:
    void error(const QString& msg);
//#ifdef DEBUG_MOD
    void resultDebug(const QVector<FaceInfo>& faces);
//#endif
};

#endif // INVOLVEMENTESTIMATOR_H
