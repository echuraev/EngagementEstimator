#include <QApplication>
#include <QQmlApplicationEngine>

#include <QLocale>
#include <QTranslator>
#include <QQuickView>
#include <QQmlContext>

#include "involvementestimator.h"
#include "debuginfodrawer.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);

    qmlRegisterType<InvolvementEstimator>("ru.hse.involvementEstimator", 1, 0, "InvolvementEstimator");
    qmlRegisterType<DebugInfoDrawer>("ru.hse.debugInfoDrawer", 1, 0, "DebugInfoDrawer");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "InvolvementEstimator_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);


    //InvolvementEstimator estimator;
    //QQmlContext *context = engine.rootContext();
    //context->setContextProperty("involvementEstimator", &estimator);

    return app.exec();
}
