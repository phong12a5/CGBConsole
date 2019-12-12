#include "LDCommand.h"
#include "AppMain.h"
#include <iostream>
#include <QFile>
#include <QDir>
#include <QMutex>

#define APP_MODEL AppModel::instance()

LDCommand* LDCommand::m_instance = nullptr;

LDCommand *LDCommand::instance()
{
    if(m_instance == nullptr){
        QMutex mutex;
        mutex.lock();
        if(m_instance == nullptr){
            m_instance = new LDCommand();
        }
        mutex.unlock();
    }
    return m_instance;
}

bool LDCommand::runLDCommand(QString args, int timeout)
{
    QString cmd = QString("\"%1/dnconsole.exe\" %2").arg(APP_MODEL->ldIntallFolder()).arg(args);
    LOG << "Cmd: " << cmd;

    QProcess m_process;
    m_process.start(cmd);
    m_process.waitForFinished(timeout);
    QString output = m_process.readAllStandardOutput();
    QString error = m_process.readAllStandardError();
    if(error != "")
        LOG << "error: " << error;
    if(output != "")
        LOG << "output: " << output;
    if(error != ""){
        LOG << error;
        return false;
    }else{
        return true;
    }
}

bool LDCommand::runLDCommand(QString args, QString &output, QString &error, int timeout)
{
    QString cmd = QString("\"%1/dnconsole.exe\" %2").arg(APP_MODEL->ldIntallFolder()).arg(args);
    LOG << "Cmd: " << cmd;

    QProcess m_process;
    m_process.start(cmd);
    m_process.waitForFinished(timeout);
    output = m_process.readAllStandardOutput();
    error = m_process.readAllStandardError();
    if(error != "")
        LOG << "error: " << error;
    if(output != "")
        LOG << "output: " << output;
    if(error != ""){
        return false;
    }else{
        return true;
    }
}

bool LDCommand::lunchInstance(QString instanceName)
{
    LOG << instanceName;
    QMutex mutex;
    mutex.lock();
    bool success = this->runLDCommand(QString("launch --name %1").arg(instanceName));
    mutex.unlock();
    return success;
}

bool LDCommand::runApp(QString instanceName, QString packageName)
{
    LOG << "instanceName: " << instanceName << " -- packageName: " << packageName;
    QMutex mutex;
    mutex.lock();
    bool success = this->runLDCommand(QString("runapp --name %1 --packagename %2").arg(instanceName).arg(packageName));
    mutex.unlock();
    return success;
}

bool LDCommand::addInstance(QString instanceName)
{
    LOG << instanceName;
    QMutex mutex;
    mutex.lock();
    bool success = this->runLDCommand(QString("add --name %1").arg(instanceName));
    mutex.unlock();
    return success;
}

QString LDCommand::ld_adb_command(QString instanceName, QString cmd, int timeout)
{
    QMutex mutex;
    mutex.lock();
    QString output, error, retVal;
    this->runLDCommand(QString("adb --name %1 --command \"%2\"").arg(instanceName).arg(cmd), output, error,timeout);
    if(error != ""){
        retVal = error;
    }else{
        retVal = output;
    }
    if(output.contains("ADB server didn't")){
        QProcess::execute("Taskkill /IM adb.exe /F");
        QProcess::execute("LDSetup\adb.exe kill-server");
        QProcess::execute("LDSetup\adb.exe start-server");
    }
    mutex.unlock();
    return  retVal;
}

bool LDCommand::quitInstance(QString instanceName)
{
    LOG << instanceName;
    QMutex mutex;
    mutex.lock();
    bool success = this->runLDCommand(QString("quit --name %1").arg(instanceName));
    mutex.unlock();
    return success;
}

bool LDCommand::quitAll()
{
    LOG;
    QMutex mutex;
    mutex.lock();
    bool success = this->runLDCommand("quitall");
    mutex.unlock();
    return success;
}

bool LDCommand::rebootInstance(QString instanceName)
{
    LOG << instanceName;
    QMutex mutex;
    mutex.lock();
    bool success = this->runLDCommand(QString("reboot --name %1").arg(instanceName));
    mutex.unlock();
    return success;
}

bool LDCommand::checkConnection(QString instanceName)
{
#ifdef USE_FILE_STEADOF_ADB
    QString targetFileName = instanceName + ".checker";
    QFile::remove(targetFileName);
    if(this->runLDCommand(QString("pull --name %1 --remote /sdcard/Pictures/.checkconnect.st --local ./%2").arg(instanceName).arg(targetFileName))){
        if(QFile(targetFileName).exists()){
            QFile::remove(targetFileName);
            return true;
        }
    }
    return false;
#else
    QString output;
    output = ld_adb_command(instanceName,"shell ls | grep sdcard");
    output = output.simplified();
    if(output == "sdcard"){
        LOG << QString("Connect to %1: successful").arg(instanceName);
        return true;
    }else{
        return false;
    }
#endif
}

bool LDCommand::coppyInstance(QString instanceName, QString fromInstanceName)
{
    LOG << instanceName << " from "  << fromInstanceName;
    QMutex mutex;
    mutex.lock();
    bool success = this->runLDCommand(QString("copy --name %1 --from %2").arg(instanceName).arg(fromInstanceName));
    mutex.unlock();
    return success;
}

bool LDCommand::isAppRunning(QString instanceName)
{
    return ld_adb_command(instanceName,QString("shell ps | grep %1").arg(FARM_PACKAGE_NAME)).contains(FARM_PACKAGE_NAME);
}

bool LDCommand::sortWindow()
{
    QMutex mutex;
    mutex.lock();
    bool success = this->runLDCommand("sortWnd");
    mutex.unlock();
    return success;
}

bool LDCommand::isExistedPackage(QString instanceName,QString packageName)
{
#if 0
    QString output = ld_adb_command(instanceName,QString("shell pm list packages | grep %1").arg(packageName));
    if(output.contains(packageName))
        return true;
    else
        return false;
#endif
    Q_UNUSED(instanceName)
    QFile file("LDSetup/data/apps.text");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        LOG << "Open file fail";
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
    QMutex mutex;
    mutex.lock();
    bool success = this->runLDCommand(QString("push --name %1 --remote %2 --local %3").arg(instanceName).arg(filePath).arg(target));
    mutex.unlock();
    return success;
}

bool LDCommand::pullFile(QString instanceName, QString remoteFile, QString localFile)
{
    QMutex mutex;
    mutex.lock();
    bool success = this->runLDCommand(QString("pull --name %1 --remote %2 --local %3").arg(instanceName).arg(remoteFile).arg(localFile));
    mutex.unlock();
    return success;
}

int LDCommand::isRunningDevice(QString instanceName)
{
    QMutex mutex;
    mutex.lock();
    int retVal = DEVICE_STATE::DEVICE_STATE_UNKNOW;
    QString output, error;
    if(this->runLDCommand(QString("isrunning --name %1").arg(instanceName),output,error)){
        if(output.simplified() == "running")
            retVal = DEVICE_STATE::DEVICE_STATE_RUNNING;
        else if (output.simplified() == "stop") {
            retVal = DEVICE_STATE::DEVICE_STATE_STOP;
        }
    }
    mutex.unlock();
    return retVal;
}

bool LDCommand::repairEmulator()
{
    QString exeFileName = QDir::toNativeSeparators("\"" + QDir::currentPath() + "/LDSetup/dnrepairer.exe\"");
    int result = (int)::ShellExecuteA(nullptr, "runas", exeFileName.toUtf8().constData(), nullptr, nullptr, SW_SHOWNORMAL);
    LOG << "result: " << result;
    return (result >= 32? true : false);
}

bool LDCommand::isExistedDevice(QString instanceName)
{
    QString deviceList;
    QString error;
    if(this->runLDCommand("list", deviceList, error)){
        if(deviceList.contains(instanceName)){
            LOG << "Device " << instanceName << " existed already";
            return  true;
        } else {
            return false;
        }
    }else {
        return false;
    }
}
