#ifndef INVOLVEMENTESTIMATOR_H
#define INVOLVEMENTESTIMATOR_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <QPixmap>
//#ifdef DEBUG_MOD
#include <QVector>
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
    //void resultDebug(const QVector<QMLFace>& faces);
    void resultDebug(const QVector<QString>& label, const QVector<float>& x1,
                     const QVector<float>& y1, const QVector<float>& x2, const QVector<float>& y2);
//#endif
};

#endif // INVOLVEMENTESTIMATOR_H
