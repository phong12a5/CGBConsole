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
    static bool lunchInstance(QString instanceName);
    static bool runApp(QString instanceName, QString packageName);
    static bool killApp(QString instanceName, QString packageName);
    static bool addInstance(QString instanceName);
    static QString ld_adb_command(QString instanceName, QString cmd, int timeout = -1);
    static bool quitInstance(QString instanceName);
    static bool quitAll();
    static bool rebootInstance(QString instanceName);
    static bool checkConnection(QString instanceName);
    static bool checkEnscript(QString instanceName);
    static bool coppyInstance(QString instanceName, QString fromInstanceName);
    static bool isAppRunning(QString instanceName);
    static bool sortWindow();
    static bool isExistedPackage(QString instanceName,QString packageName);
    static bool pushFile(QString instanceName, QString filePath, QString target);
    static bool pullFile(QString instanceName, QString filePath, QString target);
    static int isRunningDevice(QString instanceName);
    static bool repairEmulator();
    static bool isExistedDevice(QString instanceName);
    static bool renameDevice(QString deviceNameOld, QString deviceNameNew);
    static int bindWinId(QString instanceName);
    static int topWinId(QString instanceName);
    static bool installApk(QString instanceName, QString apkPath);
    static bool enableFElement(QString instanceName);
    static QString getElements(QString instanceName);

private:

};

#endif // LDCOMMAND_H
