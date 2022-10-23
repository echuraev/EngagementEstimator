#ifndef INVOLVEMENTESTIMATOR_H
#define INVOLVEMENTESTIMATOR_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <QPixmap>

#ifdef DEBUG_MOD
struct DebugInfo;
#endif

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
#ifdef DEBUG_MOD
    void resultDebug(const DebugInfo& debugInfo);
#endif
};

#endif // INVOLVEMENTESTIMATOR_H
