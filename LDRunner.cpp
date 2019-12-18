#include "LDRunner.h"
#include <QThread>
#include "LDCommand.h"
#include "AppModel.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QTime>
#include <QStandardPaths>

#define APP_MODEL AppModel::instance()

LDRunner::LDRunner(QString instanceName):
    m_instanceName(instanceName),
    m_deviceStatus(LDService::E_DEVICE_DISCONNECT),
    m_appStatus(LDService::E_APP_STOPPED),
    m_missionStatus(LDService::E_MISSION_INCOMPLETED)
{
    /* Remove check running file */
    QString runningFileName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/LDPlayer/Applications/" + "." + instanceName + ".running";
    if(QFile(runningFileName).exists()){
        QFile::remove(runningFileName);
    }

    /* Remove check running file */
    QString endScriptFileName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/LDPlayer/Applications/" + "." + instanceName + ".endscript";
    if(QFile(endScriptFileName).exists()){
        QFile::remove(endScriptFileName);
    }

    QString resolution = APP_MODEL->resolution();
    QStringList listResoltion = QStringList() << "540,960,240" << "720,1280,320" << "900,1600,320" << "1080,1920,490";
    if(resolution == "Random"){
        resolution = listResoltion.at(QTime::currentTime().msec()%4);
    }
    LDCommand::instance()->runLDCommand(QString("modify --name %1 --cpu 1 --memory 1024 --resolution %2").arg(m_instanceName).arg(resolution));
    LOGD << "Created Runner for " << instanceName;
}

LDRunner::~LDRunner()
{
    LOGD << m_instanceName;
    LDCommand::instance()->quitInstance(m_instanceName);
}

void LDRunner::run()
{
    LOGD << "Thread ID: " << QThread::currentThreadId();

    connect(LDService::instance(), &LDService::updateDeviceStatus,  this,                   &LDRunner::onUpdateDeviceStatus);
    connect(LDService::instance(), &LDService::updateAppStatus,     this,                   &LDRunner::onUpdateAppStatus);
    connect(LDService::instance(), &LDService::updateMissionStatus, this,                   &LDRunner::onUpdateMissionStatus);
    connect(this,                  &LDRunner::passConfigToDevice,   LDService::instance(),  &LDService::onPassConfigToDevice);

    m_checkRunningDevice = new QTimer(this);
    m_checkRunningDevice->setInterval(180000);
    m_checkRunningDevice->setSingleShot(false);
    connect(m_checkRunningDevice,SIGNAL(timeout()),this,SLOT(onCheckRunningDevice()));

    LDCommand::instance()->lunchInstance(m_instanceName);

    m_checkLifeTcycle = new QTimer(this);
    m_checkLifeTcycle->setInterval(1800000);
    m_checkLifeTcycle->setSingleShot(false);
    connect(m_checkLifeTcycle,SIGNAL(timeout()),this,SLOT(onCheckLifeTcycle()));

#if 0
    QTimer::singleShot(3000, this, [] () { LDCommand::instance()->sortWindow();});
#endif
    m_checkRunningDevice->start();
    m_checkLifeTcycle->start();
}

void LDRunner::onCheckRunningDevice()
{
    int deviceState = LDCommand::instance()->isRunningDevice(m_instanceName) ;
    if(deviceState == LDCommand::instance()->DEVICE_STATE_RUNNING) {
        LOGD << m_instanceName << " run already!";
        if(m_deviceStatus != LDService::E_DEVICE_CONNECTED){
            LDCommand::instance()->rebootInstance(m_instanceName);
        }
    }else if(deviceState == LDCommand::instance()->DEVICE_STATE_STOP){
        LOGD << m_instanceName << " is not running now!";
        LDCommand::instance()->lunchInstance(m_instanceName);
    }else {
        LOGD << "Could not determine state of " <<  m_instanceName;
    }
}

void LDRunner::onCheckLifeTcycle()
{
    LOGD;
    emit finished();
}

void LDRunner::onUpdateDeviceStatus(QMap<QString,LDService::E_DEVICE_STATUS> *listDeviceStatus)
{
    if(listDeviceStatus->contains(m_instanceName)){
        if (m_deviceStatus != listDeviceStatus->value(m_instanceName) && m_deviceStatus != LDService::E_DEVICE_CONNECTED) {
            m_deviceStatus = listDeviceStatus->value(m_instanceName);
            LOGD << m_instanceName << " is connected";
            emit passConfigToDevice(m_instanceName);
        }
    }
}

void LDRunner::onUpdateAppStatus(QMap<QString,LDService::E_APP_STATUS> *listAppStatus)
{
    if(listAppStatus->contains(m_instanceName)){
        if (listAppStatus->value(m_instanceName) != LDService::E_APP_RUNNING && \
                m_appStatus != LDService::E_APP_RUNNING && \
                m_deviceStatus == LDService::E_DEVICE_CONNECTED)
        {
            m_appStatus = listAppStatus->value(m_instanceName);
            LDCommand::instance()->runApp(m_instanceName, FARM_PACKAGE_NAME);
        }else if(listAppStatus->value(m_instanceName) == LDService::E_APP_RUNNING){
            LOGD << m_instanceName << " xyz.autofarmer.app started!";
        }
    }
}

void LDRunner::onUpdateMissionStatus(QMap<QString,LDService::E_MISSION_STATUS> *listMissionpStatus)
{
    if(listMissionpStatus->contains(m_instanceName)){
        m_missionStatus = listMissionpStatus->value(m_instanceName);
        if(m_missionStatus == LDService::E_MISSION_COMPLETED){
            LOGD << m_instanceName << ": Mission completed!";
            emit finished();
        }
    }
}

