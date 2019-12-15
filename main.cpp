#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "AppMain.h"
#include <QtWidgets>
#include <iostream>
#include <LDService.h>

int main(int argc, char *argv[])
{
    QProcess::execute("Taskkill /IM adb.exe /F");
    QProcess::execute("Taskkill /IM ld.exe /F");
    QProcess::execute("Taskkill /IM dnconsole.exe /F");
    QProcess::execute("Taskkill /IM dnplayer.exe /F");

    LDCommand::instance()->repairEmulator();

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    AppMain appMain;
    appMain.initApplication();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("AppModel",AppModel::instance());
    engine.rootContext()->setContextProperty("AppMain",&appMain);
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
