#include <QGuiApplication>
#include "AppMain.h"
#include <QtWidgets>
#include <iostream>


int main(int argc, char *argv[])
{
    QProcess::execute("Taskkill /IM adb.exe /F");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    LDCommand::repairEmulator();

    AppMain appMain;
    appMain.initApplication();

    return app.exec();
}
