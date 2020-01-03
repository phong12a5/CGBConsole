#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "AppMain.h"
#include <QtWidgets>
#include <iostream>
#include <LDService.h>
#include <CkGlobal.h>

CkGlobal glob;

bool unlockChilkat();

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    if (unlockChilkat()){
        LLOGD("unlockChilkat successfully");
    } else {
        LLOGD("unlockChilkat Failure");
    }

    AppModel::instance();

    QProcess::execute("Taskkill /IM adb.exe /F");
    QProcess::execute("Taskkill /IM ld.exe /F");
    QProcess::execute("Taskkill /IM dnconsole.exe /F");
    QProcess::execute("Taskkill /IM dnplayer.exe /F");

    LDCommand::instance()->repairEmulator();


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


bool unlockChilkat() {
    bool success_global = glob.UnlockBundle("VONGTH.CB4082020_9kru5rnD5R2h");
    if (success_global != true) {
        LLOGD("Error: " + QString(glob.lastErrorText()));
        return false;
    }

    int status = glob.get_UnlockStatus();
    if (status == 2) {
        return false;
    } else {
        return false;
    }
}
