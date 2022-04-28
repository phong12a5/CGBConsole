#include "LDCommand.h"
#include "AppMain.h"
#include <iostream>
#include <QFile>
#include <QDir>
//#include <//QMutex>
#include <QJsonDocument>
#include <QJsonObject>
#include <math.h>
#include <QStandardPaths>
#include <QRegularExpression>


#define APP_MODEL AppModel::instance()

static QString s_installationPath = "";

void LDCommand::setLDPath(QString LDInstallationPath)
{
    s_installationPath = LDInstallationPath;
}

bool LDCommand::runLDCommand(QString args, int timeout)
{
    QString cmd = QString("\"%1/ldconsole.exe\" %2").arg(s_installationPath).arg(args);
    LOGD("Cmd: " + cmd);

    QProcess m_process;
    m_process.start(cmd);
    m_process.waitForFinished(timeout);
    QString output = m_process.readAllStandardOutput();
    QString error = m_process.readAllStandardError();
    if(output != "") {
//        LOGD("output: " + output);
    }

    if(error != ""){
        LOGD(error);
        return false;
    }else{
        return true;
    }
}

bool LDCommand::runLDCommand(QString args, QString &output, QString &error, int timeout)
{
    QString cmd = QString("\"%1/ldconsole.exe\" %2").arg(s_installationPath).arg(args);
    LOGD("Cmd: " + cmd);

    QProcess m_process;
    m_process.start(cmd);
    m_process.waitForFinished(timeout);
    output = m_process.readAllStandardOutput();
    error = m_process.readAllStandardError();
    if(output != "") {
//        LOGD("output: " + output);
    }

    if(error != ""){
        LOGD("error: " + error);
        return false;
    } else {
        return true;
    }
}

bool LDCommand::lunchInstance(int index)
{
    return runLDCommand(QString("launch --index %1").arg(index));
}

bool LDCommand::runApp(int index, QString packageName)
{
    return runLDCommand(QString("runapp --index %1 --packagename %2").arg(index).arg(packageName));
}

bool LDCommand::killApp(int index, QString packageName)
{
   return runLDCommand(QString("killapp --index %1 --packagename %2").arg(index).arg(packageName));
}

bool LDCommand::addInstance(int index)
{
    return runLDCommand(QString("add --index %1").arg(index));
}

QString LDCommand::ld_adb_command(int index, QString cmd, int timeout)
{
    QString output, error;
    runLDCommand(QString("adb --index %1 --command \"%2\"").arg(index).arg(cmd), output, error,timeout);
    if(error != ""){
        LOGD(error);
    }
    if(output.contains("ADB server didn't")){
//        QProcess::execute("Taskkill /IM adb.exe /F");
//        QProcess::execute("LDSetup\adb.exe kill-server");
//        QProcess::execute("LDSetup\adb.exe start-server");
        LOGD(QString::number(index) + ": " + output);
    }

    if(output.contains("error: device not found")) {
        LOGD(QString::number(index) + ": " + output);
        LOGD("retry to connect: " + QString::number(index));
        if(connectADB(index)) {
            runLDCommand(QString("adb --index %1 --command \"%2\"").arg(index).arg(cmd), output, error,timeout);
        }
    }
    return  output;
}

bool LDCommand::quitInstance(int index)
{
    bool success = runLDCommand(QString("quit --index %1").arg(index));
    return success;
}

bool LDCommand::quitAll()
{
    bool success = runLDCommand("quitall");
    return success;
}

bool LDCommand::rebootInstance(int index)
{
    bool success = runLDCommand(QString("reboot --index %1").arg(index));
    return success;
}

bool LDCommand::coppyInstance(int index, int fromIndex)
{
    bool success = runLDCommand(QString("copy --index %1 --from %2").arg(index).arg(fromIndex));
    return success;
}

bool LDCommand::isAppInForeground(int index, QString packageName)
{
    QString output = ld_adb_command(index,"shell dumpsys window windows | grep -E 'mFocusedApp'| cut -d / -f 1 | cut -d ' ' -f 7");
    return output.simplified() == packageName;
}

bool LDCommand::sortWindow()
{
    return runLDCommand("sortWnd");
}

bool LDCommand::pushFile(int index, QString filePath, QString target)
{
    QString fileTemp = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/LDPlayer/Pictures/temp";
    if(QFile(fileTemp).exists()){
        QFile::remove(fileTemp);
    }
    bool success = runLDCommand(QString("push --index %1 --remote %2 --local %3").arg(index).arg(filePath).arg(target));
    return success;
}

bool LDCommand::pullFile(int index, QString remoteFile, QString localFile)
{
    QString fileTemp = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/LDPlayer/Pictures/temp";
    if(QFile(fileTemp).exists()){
        QFile::remove(fileTemp);
    }
    bool success = runLDCommand(QString("pull --index %1 --remote %2 --local %3").arg(index).arg(remoteFile).arg(localFile));
    return success;
}

int LDCommand::isRunningDevice(int index)
{
    int retVal = DEVICE_STATE::DEVICE_STATE_UNKNOW;
    QString output, error;
    if(runLDCommand(QString("isrunning --index %1").arg(index),output,error)){
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
    return (result >= 32? true : false);
}

bool LDCommand::isExistedDevice(int index)
{
    QString deviceList;
    QString error;
    if(runLDCommand("list", deviceList, error)){
        if(deviceList.contains(index)){
            LOGD("Device " + QString::number(index) + " existed already");
            return  true;
        } else {
            return false;
        }
    }else {
        return false;
    }
}

int LDCommand::bindWinId(int index)
{
    QString deviceListStr;
    QString error;
    if(runLDCommand("list2", deviceListStr, error)){
        QStringList deviceList = deviceListStr.split("\r\n");
        foreach(QString device, deviceList) {
            QStringList params = device.split(",");
            if(params.length() == 7) {
                bool ok;
                int tmpIndex = params[0].toInt(&ok);
                if(ok && tmpIndex == index)
                    return params[3].toInt();
            }
        }
    }
    return -1;
}

int LDCommand::topWinId(int index)
{
    QString deviceListStr;
    QString error;
    if(runLDCommand("list2", deviceListStr, error)){
        QStringList deviceList = deviceListStr.split("\r\n");
        foreach(QString device, deviceList) {
            QStringList params = device.split(",");
            if(params.length() == 7) {
                bool ok;
                int tmpIndex = params[0].toInt(&ok);
                if(ok && tmpIndex == index)
                    return params[2].toInt();
            }
        }
    }
    return -1;
}

bool LDCommand::installApk(int index, QString apkPath)
{
    bool success = runLDCommand(QString("installapp --index %1 --filename %2").arg(index).arg(apkPath));
    return success;
}

bool LDCommand::enableFElement(int index)
{
    ld_adb_command(index,"shell settings put secure enabled_accessibility_services com.cgb.support/.service.QAccessibilityService", 100000);
    return true;
}

QString LDCommand::getElements(int index)
{
    return ld_adb_command(index,"shell am broadcast -a com.cgb.support.SCREEN_ELEMENT_ACTION com.cgb.support", 100000);
}

bool LDCommand::connectADB(int index)
{
    QString vmboxPath = s_installationPath + (s_installationPath.endsWith("/")? "" : "/") + "vms/leidian" + QString::number(index) + "/leidian.vbox";
    LOGD(vmboxPath);
    QFile vmbox(vmboxPath);
    if (vmbox.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString config = vmbox.readAll();
        QRegExp regex("\"Rule 1\" proto=\"1\" hostport=\"(\\d{4})\"");
        if(regex.indexIn(config) >= 0) {
            bool ok;
            int hostPort = regex.cap(1).toInt(&ok);
            if(ok) {
                QString cmd = QString("\"%1/adb.exe\" connect 127.0.0.1:%2").arg(s_installationPath).arg(hostPort);
                LOGD(cmd);
                QProcess m_process;
                m_process.start(cmd);
                m_process.waitForFinished(10000);
                QString out = m_process.readAllStandardOutput();
                LOGD(out);
                if(out.contains("connected to")) return true;
            }
        }
    }
    return false;
}
