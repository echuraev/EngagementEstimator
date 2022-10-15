#ifndef INVOLVEMENTESTIMATOR_H
#define INVOLVEMENTESTIMATOR_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <QPixmap>

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
};

#endif // INVOLVEMENTESTIMATOR_H
