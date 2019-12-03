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
    static bool runLDCommand(QString args, int timeout = -1);
    static bool runLDCommand(QString args, QString &output, QString &error, int timeout = -1);
    static bool lunchInstance(QString instanceName);
    static bool runApp(QString instanceName, QString packageName);
    static bool addInstance(QString instanceName);
    static bool ld_adb_command(QString instanceName, QString cmd, int timeout = -1);
    static QString ld_adb_command_str(QString instanceName, QString cmd, int timeout = -1);
    static bool quitInstance(QString instanceName);
    static bool quitAll();
    static bool rebootInstance(QString instanceName);
    static bool checkConnection(QString instanceName);
    static bool coppyInstance(QString instanceName, QString fromInstanceName);
    static bool isAppRunning(QString instanceName);
    static bool sortWindow();
    static bool isExistedPackage(QString packageName);
    static bool pushFile(QString instanceName, QString filePath, QString target);
    static bool pullFile(QString instanceName, QString filePath, QString target);
    static int isRunningDevice(QString instanceName);
    static bool repairEmulator();
signals:

public slots:
};

#endif // LDCOMMAND_H
