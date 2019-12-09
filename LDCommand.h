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
    static LDCommand* instance();
    bool runLDCommand(QString args, int timeout = -1);
    bool runLDCommand(QString args, QString &output, QString &error, int timeout = -1);
    bool lunchInstance(QString instanceName);
    bool runApp(QString instanceName, QString packageName);
    bool addInstance(QString instanceName);
    QString ld_adb_command(QString instanceName, QString cmd, int timeout = -1);
    bool quitInstance(QString instanceName);
    bool quitAll();
    bool rebootInstance(QString instanceName);
    bool checkConnection(QString instanceName);
    bool coppyInstance(QString instanceName, QString fromInstanceName);
    bool isAppRunning(QString instanceName);
    bool sortWindow();
    bool isExistedPackage(QString instanceName,QString packageName);
    bool pushFile(QString instanceName, QString filePath, QString target);
    bool pullFile(QString instanceName, QString filePath, QString target);
    int isRunningDevice(QString instanceName);
    bool repairEmulator();

private:
    static LDCommand* m_instance;


};

#endif // LDCOMMAND_H
