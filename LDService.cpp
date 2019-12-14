#include "LDService.h"
#include <AppModel.h>
#include <QDir>
#include <QStandardPaths>
#include <QString>
#include <AppModel.h>
#include <QJsonObject>
#include <QJsonDocument>

LDService* LDService::m_instance = nullptr;

LDService::LDService(QObject *parent) : QObject(parent),
  m_listDeviceStatus(new QMap<QString,E_DEVICE_STATUS>),
  m_listAppStatus(new QMap<QString,E_APP_STATUS>),
  m_listMissionStatus(new QMap<QString,E_MISSION_STATUS>)
{
    m_checkConnectTimer.setSingleShot(false);
    m_checkConnectTimer.setInterval(60000);
    connect(&m_checkConnectTimer, &QTimer::timeout, this, &LDService::onCheckDeviceStatus);

    m_checkRunAppTimer.setSingleShot(false);
    m_checkRunAppTimer.setInterval(60000);
    connect(&m_checkRunAppTimer, &QTimer::timeout, this, &LDService::onCheckRunApp);

    m_checkMissionSttTimer.setSingleShot(false);
    m_checkMissionSttTimer.setInterval(60000);
    connect(&m_checkMissionSttTimer, &QTimer::timeout, this, &LDService::onCheckMissionStatus);
}

LDService::~LDService()
{
    delete m_listDeviceStatus;
    delete m_listAppStatus;
    delete m_listMissionStatus;
}

LDService *LDService::instance()
{
    if(m_instance == nullptr){
        m_instance = new LDService();
    }
    return m_instance;
}

void LDService::startService()
{
    LOGD;
    m_checkConnectTimer.start();
    m_checkRunAppTimer.start();
    m_checkMissionSttTimer.start();
}

void LDService::stopService()
{
    m_checkConnectTimer.stop();
    m_checkRunAppTimer.stop();
    m_checkMissionSttTimer.stop();
}

void LDService::onCheckDeviceStatus()
{
    LOGD;
    QStringList existedDevices;
    existedDevices.clear();
    for (int i = 0; i < AppModel::instance()->devicesRunningList().length(); i++) {
        existedDevices << dynamic_cast<LDIntance*>(AppModel::instance()->devicesRunningList().at(i))->instanceName();
    }

    m_listDeviceStatus->clear();

    foreach(QString instanceName, existedDevices){
        if(LDCommand::instance()->checkConnection(instanceName)){
            m_listDeviceStatus->insert(instanceName,E_DEVICE_STATUS::E_DEVICE_CONNECTED);
        }else {
            m_listDeviceStatus->insert(instanceName,E_DEVICE_STATUS::E_DEVICE_DISCONNECT);
        }
    }
    LOGD << *m_listDeviceStatus;
    emit updateDeviceStatus(m_listDeviceStatus);
}

void LDService::onCheckRunApp()
{
    LOGD;
    QStringList existedDevices;
    existedDevices.clear();
    for (int i = 0; i < AppModel::instance()->devicesRunningList().length(); i++) {
        existedDevices << dynamic_cast<LDIntance*>(AppModel::instance()->devicesRunningList().at(i))->instanceName();
    }

    m_listAppStatus->clear();

    QDir directory(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/LDPlayer/Applications");
    QStringList runningFileList = directory.entryList(QStringList() << "*.running",QDir::Files);

    foreach(QString instanceName, existedDevices){
        if(runningFileList.contains("." + instanceName + ".running")){
            m_listAppStatus->insert(instanceName,E_APP_STATUS::E_APP_RUNNING);
        }else {
            m_listAppStatus->insert(instanceName,E_APP_STATUS::E_APP_STOPPED);
        }
    }
    emit updateAppStatus(m_listAppStatus);
}

void LDService::onCheckMissionStatus()
{
    LOGD;
    QStringList existedDevices;
    existedDevices.clear();
    for (int i = 0; i < AppModel::instance()->devicesRunningList().length(); i++) {
        existedDevices << dynamic_cast<LDIntance*>(AppModel::instance()->devicesRunningList().at(i))->instanceName();
    }

    m_listMissionStatus->clear();

    QDir directory(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/LDPlayer/Applications");
    QStringList runningFileList = directory.entryList(QStringList() << "*.endscript",QDir::Files);

    foreach(QString instanceName, existedDevices){
        if(runningFileList.contains("." + instanceName + ".endscript")){
            m_listMissionStatus->insert(instanceName,E_MISSION_STATUS::E_MISSION_COMPLETED);
        }else {
            m_listMissionStatus->insert(instanceName,E_MISSION_STATUS::E_MISSION_INCOMPLETED);
        }
    }
    emit updateMissionStatus(m_listMissionStatus);
}

void LDService::onPassConfigToDevice(QString instanceName)
{
    LOGD << instanceName << " Passing token id .." << AppModel::instance()->token();

    /* Create startup.config and pass to LD */
    QJsonObject configObj;
    configObj["token"] = AppModel::instance()->token();
    configObj["auto_startup"] = true;
    configObj["timeout"] = 30;
    configObj["country"] = "Vietnam";
    configObj["appname"] = AppModel::instance()->appName();
    configObj["devicename"] = instanceName;

    QString startUpFile = "startup.config";
    QFile jsonFile(startUpFile);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(QJsonDocument(configObj).toJson());
    jsonFile.close();

    LDCommand::instance()->pushFile(instanceName,QString(APP_DATA_FOLDER) + startUpFile,"./" + startUpFile);
    QFile::remove(startUpFile);
    /* Created startup.config and passed to Nox*/
}
