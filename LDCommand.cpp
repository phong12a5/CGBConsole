#include "LDCommand.h"
#include "AppMain.h"
#include <iostream>
#include <QFile>

#define APP_MODEL AppModel::instance()

bool LDCommand::runLDCommand(QString args)
{
    QString cmd = QString("\"%1/ldconsole.exe\" %2").arg(APP_MODEL->ldIntallFolder()).arg(args);
    QProcess process;
    process.start(cmd);
    process.waitForFinished(-1);
    if(process.readAllStandardError() != ""){
        LOG << process.readAllStandardError();
        return false;
    }else{
        return true;
    }
}

bool LDCommand::runLDCommand(QString args, QString &output, QString &error)
{
    QString cmd = QString("\"%1/ldconsole.exe\" %2").arg(APP_MODEL->ldIntallFolder()).arg(args);
    QProcess process;
    process.start(cmd);
    process.waitForFinished(-1);
    output = process.readAllStandardOutput();
    error = process.readAllStandardError();
    if(error != ""){
        return false;
    }else{
        return true;
    }
}

bool LDCommand::lunchInstance(QString instanceName)
{
    LOG << instanceName;
    return LDCommand::runLDCommand(QString("launch --name %1").arg(instanceName));
}

bool LDCommand::installPackage(QString instanceName, QString apkPath)
{
    LOG << "instanceName: " << instanceName << " -- apkPath: " << apkPath;
    if(!QFile(apkPath).exists()){
        LOG << "apkPath has not existed!";
        return false;
    }
    return LDCommand::runLDCommand(QString("installapp --name %1 --filename %2").arg(instanceName).arg(apkPath));
}

bool LDCommand::runApp(QString instanceName, QString packageName)
{
    LOG << "instanceName: " << instanceName << " -- packageName: " << packageName;
    return LDCommand::runLDCommand(QString("runapp --name %1 --packagename %2").arg(instanceName).arg(packageName));
}

bool LDCommand::addInstance(QString instanceName)
{
    LOG << instanceName;
    return LDCommand::runLDCommand(QString("add --name %1").arg(instanceName)) &&
            LDCommand::runLDCommand(QString("modify --name %1 --cpu 1 --memory 1024 --resolution 720,1280,320").arg(instanceName));
}

bool LDCommand::ld_adb_command(QString instanceName, QString cmd)
{
    QString args = QString("adb --name %1 --command \"%2\"").arg(instanceName).arg(cmd);
    return LDCommand::runLDCommand(args);
}

QString LDCommand::ld_adb_command_str(QString instanceName, QString cmd)
{
    QString output, error, retVal;
    LDCommand::runLDCommand(QString("adb --name %1 --command \"%2\"").arg(instanceName).arg(cmd), output, error);
    if(error != ""){
        retVal = error;
    }else{
        retVal = output;
    }
    return  retVal;
}

bool LDCommand::quitInstance(QString instanceName)
{
    LOG << instanceName;
    return LDCommand::runLDCommand(QString("quit --name %1").arg(instanceName));
}

bool LDCommand::quitAll()
{
    LOG;
    return LDCommand::runLDCommand("quitall");
}

bool LDCommand::rebootInstance(QString instanceName)
{
    LOG << instanceName;
    return LDCommand::runLDCommand(QString("reboot --name %1").arg(instanceName));
}

bool LDCommand::checkConnection(QString instanceName)
{
    QString output;
    output = LDCommand::ld_adb_command_str(instanceName,"shell ls | grep sdcard");
    output = output.simplified();
    if(output == "sdcard"){
        return true;
    }else{
        return false;
    }
}

bool LDCommand::coppyInstance(QString instanceName, QString fromInstanceName)
{
    LOG << instanceName << " from "  << fromInstanceName;
    return LDCommand::runLDCommand(QString("copy --name %1 --from %2").arg(instanceName).arg(fromInstanceName));
}

QString LDCommand::currentActivity(QString instanceName)
{
    QString retVal;
    QStringList output = LDCommand::ld_adb_command_str(instanceName,"shell dumpsys window windows | grep -E 'mCurrentFocus'").split(' ');
    if(output.length() > 4){
        retVal = output.at(4).simplified().remove('}');
    }
    LOG << "retVal: " << retVal;
    return retVal;
}

bool LDCommand::isAppRunning(QString instanceName)
{
    bool retVal = LDCommand::ld_adb_command_str(instanceName,QString("shell ps | grep %1").arg(FARM_PACKAGE_NAME)).contains(FARM_PACKAGE_NAME);
    LOG << retVal;
    return retVal;
}

bool LDCommand::isInstanceRunning(QString instanceName)
{
    LOG;
    return LDCommand::runLDCommand(QString("isrunning --name %1").arg(instanceName));
}

bool LDCommand::sortWindow()
{
    LOG;
    return LDCommand::runLDCommand("sortWnd");
}

bool LDCommand::checkPermission(QString instanceName, QString packageName, QString permission)
{
    QString output = ld_adb_command_str(instanceName,QString("shell dumpsys package %1 | grep %2").arg(packageName).arg(permission));
    if(output.contains(permission))
        return true;
    else
        return false;
}
