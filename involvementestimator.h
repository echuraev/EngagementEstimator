#ifndef INVOLVEMENTESTIMATOR_H
#define INVOLVEMENTESTIMATOR_H

#include <QObject>
#include <QPixmap>

class InvolvementEstimator : public QObject
{
    Q_OBJECT
public:
    explicit InvolvementEstimator(QObject *parent = nullptr);

public slots:
    void run(int x, int y, int width, int height);
};

#endif // INVOLVEMENTESTIMATOR_H
