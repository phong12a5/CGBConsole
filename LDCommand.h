#ifndef LDCOMMAND_H
#define LDCOMMAND_H

#include <QObject>
#include "AppDefines.h"
#include <QProcess>

class LDCommand : public QObject
{
    Q_OBJECT

public:
    static bool runLDCommand(QString args, int timeout = -1);
    static bool runLDCommand(QString args, QString &output, QString &error, int timeout = -1);
    static bool lunchInstance(QString instanceName);
    static bool installPackage(QString instanceName, QString apkPath);
    static bool runApp(QString instanceName, QString packageName);
    static bool addInstance(QString instanceName);
    static bool ld_adb_command(QString instanceName, QString cmd, int timeout = -1);
    static QString ld_adb_command_str(QString instanceName, QString cmd, int timeout = -1);
    static bool quitInstance(QString instanceName);
    static bool quitAll();
    static bool rebootInstance(QString instanceName);
    static bool checkConnection(QString instanceName);
    static bool coppyInstance(QString instanceName, QString fromInstanceName);
    static QString currentActivity(QString instanceName);
    static bool isAppRunning(QString instanceName);
    static bool isInstanceRunning(QString instanceName);
    static bool sortWindow();
    static bool checkPermission(QString instanceName, QString packageName, QString permission);
    static bool isExistedPackage(QString instanceName, QString packageName);

signals:

public slots:
};

#endif // LDCOMMAND_H
