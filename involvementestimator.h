#ifndef INVOLVEMENTESTIMATOR_H
#define INVOLVEMENTESTIMATOR_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <QPixmap>

#include "resultinfo.h"
#include "facetracker.h"

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
    // Currently we use signal result only in DEBUG_MOD.
    // Probably something will be changed in the future
    void result(const ResultInfo& resultInfo);

private:
    FaceTracker m_faceTracker;
};

#endif // INVOLVEMENTESTIMATOR_H
