#ifndef LDCOMMAND_H
#define LDCOMMAND_H

#include <QObject>
#include "AppDefines.h"
#include <QProcess>

class LDCommand : public QObject
{
    Q_OBJECT

public:
    enum DEVICE_STATE: int {
        DEVICE_STATE_UNKNOW = 0,
        DEVICE_STATE_RUNNING,
        DEVICE_STATE_STOP
    };

public:
    static void setLDPath(QString LDInstallationPath);
    static bool runLDCommand(QString args, int timeout = -1);
    static bool runLDCommand(QString args, QString &output, QString &error, int timeout = -1);
    static bool lunchInstance(int index);
    static bool runApp(int index, QString packageName);
    static bool killApp(int index, QString packageName);
    static bool addInstance(int index);
    static QString ld_adb_command(int index, QString cmd, int timeout = 100000);
    static bool quitInstance(int index);
    static bool quitAll();
    static bool rebootInstance(int index);
    static bool coppyInstance(int index, int fromIndex);
    static bool isAppInForeground(int index, QString packageName);
    static bool sortWindow();
    static bool pushFile(int index, QString filePath, QString target);
    static bool pullFile(int index, QString filePath, QString target);
    static int isRunningDevice(int index);
    static bool repairEmulator();
    static bool isExistedDevice(int index);
    static int bindWinId(int index);
    static int topWinId(int index);
    static bool installApk(int index, QString apkPath);
    static bool enableFElement(int index);
    static QString getElements(int index);
    static bool connectADB(int index);

private:

};

#endif // LDCOMMAND_H
