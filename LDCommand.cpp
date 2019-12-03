#include "LDCommand.h"
#include "AppMain.h"
#include <iostream>
#include <QFile>
#include <QDir>

#define APP_MODEL AppModel::instance()

bool LDCommand::runLDCommand(QString args, int timeout)
{
    QString cmd = QString("\"%1/ldconsole.exe\" %2").arg(APP_MODEL->ldIntallFolder()).arg(args);
    LOG << "Cmd: " << cmd;
    QProcess process;
    process.start(cmd);
    process.waitForFinished(timeout);
    QString error = process.readAllStandardError();

    if(error != ""){
        LOG << error;
        return false;
    }else{
        return true;
    }
}

bool LDCommand::runLDCommand(QString args, QString &output, QString &error, int timeout)
{
    QString cmd = QString("\"%1/ldconsole.exe\" %2").arg(APP_MODEL->ldIntallFolder()).arg(args);
    LOG << "Cmd: " << cmd;
    QProcess process;
    process.start(cmd);
    process.waitForFinished(timeout);
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

bool LDCommand::runApp(QString instanceName, QString packageName)
{
    LOG << "instanceName: " << instanceName << " -- packageName: " << packageName;
    return LDCommand::runLDCommand(QString("runapp --name %1 --packagename %2").arg(instanceName).arg(packageName));
}

bool LDCommand::addInstance(QString instanceName)
{
    LOG << instanceName;
    return LDCommand::runLDCommand(QString("add --name %1").arg(instanceName));
}

bool LDCommand::ld_adb_command(QString instanceName, QString cmd, int timeout)
{
    QString args = QString("adb --name %1 --command \"%2\"").arg(instanceName).arg(cmd);
    return LDCommand::runLDCommand(args,timeout);
}

QString LDCommand::ld_adb_command_str(QString instanceName, QString cmd, int timeout)
{
    QString output, error, retVal;
    LDCommand::runLDCommand(QString("adb --name %1 --command \"%2\"").arg(instanceName).arg(cmd), output, error,timeout);
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
    QString startUpPath = QString("/sdcard/startup.config");
    QString startUpLocalPath = "./" + instanceName + "_" + "startup.config";
    if(LDCommand::pullFile(instanceName,startUpPath,startUpLocalPath)){
        if(QFile(startUpLocalPath).exists()){
            QFile::remove(startUpLocalPath);
            return true;
        }
    }
    return false;
}

bool LDCommand::coppyInstance(QString instanceName, QString fromInstanceName)
{
    LOG << instanceName << " from "  << fromInstanceName;
    return LDCommand::runLDCommand(QString("copy --name %1 --from %2").arg(instanceName).arg(fromInstanceName));
}

bool LDCommand::isAppRunning(QString instanceName)
{
    bool retVal = LDCommand::ld_adb_command_str(instanceName,QString("shell ps | grep %1").arg(FARM_PACKAGE_NAME)).contains(FARM_PACKAGE_NAME);
    LOG << retVal;
    return retVal;
}

bool LDCommand::sortWindow()
{
    LOG;
    return LDCommand::runLDCommand("sortWnd");
}

bool LDCommand::isExistedPackage(QString packageName)
{
    QFile file("LDSetup/data/apps.text");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        LOG << "Open file fail";
        return false;
    }
    while (!file.atEnd()) {
        QString line = QString(file.readLine());
        LOG << "line: " << line;
        if(line.contains(packageName))
            return true;
    }
    return false;
}

bool LDCommand::pushFile(QString instanceName, QString filePath, QString target)
{
    return LDCommand::runLDCommand(QString("push --name %1 --remote %2 --local %3").arg(instanceName).arg(filePath).arg(target));
}

bool LDCommand::pullFile(QString instanceName, QString remoteFile, QString localFile)
{
    return LDCommand::runLDCommand(QString("pull --name %1 --remote %2 --local %3").arg(instanceName).arg(remoteFile).arg(localFile));
}

int LDCommand::isRunningDevice(QString instanceName)
{
    int retVal = DEVICE_STATE::DEVICE_STATE_UNKNOW;
    QString output, error;
    if(LDCommand::runLDCommand(QString("isrunning --name %1").arg(instanceName),output,error)){
        if(output.simplified() == "running")
            retVal = DEVICE_STATE::DEVICE_STATE_RUNNING;
        else if (output.simplified() == "stop") {
            retVal = DEVICE_STATE::DEVICE_STATE_STOP;
        }
    }
    return retVal;
}

bool LDCommand::repairEmulator()
{
    QString exeFileName = QDir::toNativeSeparators("\"" + QDir::currentPath() + "/LDSetup/dnrepairer.exe\"");
    int result = (int)::ShellExecuteA(nullptr, "runas", exeFileName.toUtf8().constData(), nullptr, nullptr, SW_SHOWNORMAL);
    LOG << "result: " << result;
    return (result >= 32? true : false);
}
