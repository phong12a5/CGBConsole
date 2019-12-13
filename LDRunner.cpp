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
    m_setIsLDFile(false)
{
    /* Remove check running file */
    QString runningFileName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/LDPlayer/Applications/" + "." + instanceName + ".running";
    if(QFile(runningFileName).exists()){
        QFile::remove(runningFileName);
    }

    /* Remove check running file */
    QString endScriptFileName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/LDPlayer/Applications/" + "." + instanceName + ".enscript";
    if(QFile(endScriptFileName).exists()){
        QFile::remove(endScriptFileName);
    }

    QString resolution = APP_MODEL->resolution();
    QStringList listResoltion = QStringList() << "540,960,240" << "720,1280,320" << "900,1600,320" << "1080,1920,490";
    if(resolution == "Random"){
        resolution = listResoltion.at(QTime::currentTime().msec()%4);
    }
    LDCommand::instance()->runLDCommand(QString("modify --name %1 --cpu 1 --memory 1024 --resolution %2").arg(m_instanceName).arg(resolution));
}

LDRunner::~LDRunner()
{
    LOGD << m_instanceName;
    LDCommand::instance()->quitInstance(m_instanceName);
}

void LDRunner::run()
{
    LOGD << "Thread ID: " << QThread::currentThreadId();

    m_checkConnectionTimer = new QTimer(this);
    m_checkConnectionTimer->setInterval(30000);
    m_checkConnectionTimer->setSingleShot(false);
    connect(m_checkConnectionTimer,SIGNAL(timeout()),this,SLOT(onCheckConnection()));

    m_checkEndScriptTimer = new QTimer(this);
    m_checkEndScriptTimer->setInterval(30000);
    m_checkEndScriptTimer->setSingleShot(false);
    connect(m_checkEndScriptTimer,SIGNAL(timeout()),this,SLOT(onCheckEnscript()));

    m_checkRunAppTimer = new QTimer(this);
    m_checkRunAppTimer->setInterval(APP_MODEL->appConfig().m_openApkAfterNSeconds * 1000);
    m_checkRunAppTimer->setSingleShot(false);
    connect(m_checkRunAppTimer,SIGNAL(timeout()),this,SLOT(onCheckRunApp()));

    m_checkRunningDevice = new QTimer(this);
    m_checkRunningDevice->setInterval(20000);
    m_checkRunningDevice->setSingleShot(false);
    connect(m_checkRunningDevice,SIGNAL(timeout()),this,SLOT(onCheckRunningDevice()));

    LDCommand::instance()->lunchInstance(m_instanceName);
    QTimer::singleShot(3000, this, [] () { LDCommand::instance()->sortWindow();});
    m_checkRunningDevice->start();
}

void LDRunner::quitRunner()
{
    LOGD;
    m_checkConnectionTimer->stop();
    m_checkRunAppTimer->stop();
    m_checkEndScriptTimer->stop();
    m_checkRunningDevice->stop();
}

void LDRunner::onCheckConnection()
{
    if (LDCommand::instance()->checkConnection(m_instanceName) && !m_checkEndScriptTimer->isActive()){
        LOGD << m_instanceName << " is connected";

        // Set token
        LOGD << "Passing token id .." << APP_MODEL->token();
        QString value, error;

        /* Create startup.config and pass to LD */
        QJsonObject configObj;
        configObj["token"] = APP_MODEL->token();
        configObj["auto_startup"] = true;
        configObj["timeout"] = 30;
        configObj["country"] = "Vietnam";
        configObj["appname"] = APP_MODEL->appName();
        configObj["devicename"] = m_instanceName;

        QString startUpFile = "startup.config";
        QFile jsonFile(startUpFile);
        jsonFile.open(QFile::WriteOnly);
        jsonFile.write(QJsonDocument(configObj).toJson());
        jsonFile.close();

        LDCommand::instance()->pushFile(m_instanceName,QString(APP_DATA_FOLDER) + startUpFile,"./" + startUpFile);
        QFile::remove(startUpFile);
        /* Created startup.config and passed to Nox*/

        // Run app
        m_checkConnectionTimer->stop();
        m_checkRunAppTimer->start();
    }else {
        LOGD << m_instanceName << " " << false;
    }
}

void LDRunner::onCheckEnscript()
{
    if(LDCommand::instance()->checkEnscript(m_instanceName)){
        LOGD << m_instanceName << " is done";
        emit finished();
    }
}

void LDRunner::onCheckRunApp()
{
    if (!LDCommand::instance()->isAppRunning(m_instanceName)) {
        LDCommand::instance()->runApp(m_instanceName, FARM_PACKAGE_NAME);
    }else {
        LOGD << "App is run already";
        m_checkRunAppTimer->stop();
        m_checkEndScriptTimer->start();
    }
}

void LDRunner::onCheckRunningDevice()
{
    int deviceState = LDCommand::instance()->isRunningDevice(m_instanceName) ;
    if(deviceState == LDCommand::instance()->DEVICE_STATE_RUNNING) {
        LOGD << m_instanceName << " run already!";
        m_checkRunningDevice->stop();
        m_checkConnectionTimer->start();
    }else if(deviceState == LDCommand::instance()->DEVICE_STATE_STOP){
        LOGD << m_instanceName << " is not running now!";
        LDCommand::instance()->lunchInstance(m_instanceName);
    }else {
        LOGD << "Could not determine state of " <<  m_instanceName;
    }
}
