#include <QApplication>
#include <MainWindow.h>
#include "AppMain.h"
#include <QtWidgets>
#include <iostream>
#include <LDService.h>
#include <CkGlobal.h>

CkGlobal glob;

bool unlockChilkat();

int main(int argc, char *argv[])
{
    if (unlockChilkat()){
        LLOGD("unlockChilkat successfully");
    } else {
        LLOGD("unlockChilkat Failure");
    }

    QProcess::execute("Taskkill /IM adb.exe /F");
    QProcess::execute("Taskkill /IM ld.exe /F");
    QProcess::execute("Taskkill /IM dnconsole.exe /F");
    QProcess::execute("Taskkill /IM dnplayer.exe /F");

    LDCommand::repairEmulator();

    AppMain appMain;
    appMain.initApplication();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


bool unlockChilkat() {
    bool success_global = glob.UnlockBundle("AUTFRM.CB4082023_Pz2Ry7az86p4");
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
