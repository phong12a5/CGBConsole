#include "AppMain.h"
#include <QFile>
#include <WebAPI.hpp>
#include <QDir>
#include <QFile>
#include <thread>
#include <QCoreApplication>
#include <QApplication>
#include <AutoUpdaterWorker.h>
#include <QStandardPaths>
#include <QScreen>
#include <LDService.h>

#define APP_MODEL   AppModel::instance()
#define APP_CTRL    AppController::instance()
#define WEB_API     WebAPI::instance()

AppMain::AppMain(QObject *parent) :
    QObject(parent),
    m_copyInProgress(false)
{
    connect(APP_MODEL,SIGNAL(reInitDeviceList()),this,SLOT(initDevicesList()));
    connect(APP_MODEL,SIGNAL(sigStartProgram()),this,SLOT(onStartProgram()));
    connect(APP_MODEL,SIGNAL(sigStoptProgram()),this,SLOT(onStoptProgram()));

    m_emulaterWorker = new EmulatorWorker();
    m_emulaterWorker->moveToThread(&m_copyDevicesThread);

    connect(&m_copyDevicesThread, &QThread::finished, m_emulaterWorker, &QObject::deleteLater);
    connect(this, &AppMain::startCopyEmulator, m_emulaterWorker, &EmulatorWorker::onCoppyDevices);
    connect(this, &AppMain::startCreateTemplateDevice, m_emulaterWorker, &EmulatorWorker::onCreateTemplateDevice);
    connect(m_emulaterWorker, &EmulatorWorker::finishCopyDevice, this, &AppMain::onFinishCopyDevice);
    connect(m_emulaterWorker, &EmulatorWorker::finishCreateTemplateDevice, this, &AppMain::onFinishCreateTemplateDevice);
    connect(m_emulaterWorker, &EmulatorWorker::finishCopyTask, this, &AppMain::onFinishCopyTask);

    LDService::instance()->moveToThread(&m_ldServiceThread);
    connect(this, &AppMain::startLdService, LDService::instance(), &LDService::startService);
    connect(this, &AppMain::stopLdService, LDService::instance(), &LDService::stopService);

    connect(APP_MODEL, &AppModel::deviceCountChanged, this, &AppMain::copyDevices);
}

AppMain::~AppMain()
{
    m_copyDevicesThread.quit();
    m_updateVersionThread.quit();
    m_ldServiceThread.quit();
    m_copyDevicesThread.wait();
    m_updateVersionThread.wait();
    m_ldServiceThread.wait();
    LDCommand::instance()->quitAll();
    this->onSaveConfig();
}

void AppMain::initApplication()
{
    LOGD;
    this->preSetup();
    this->onLoadConfig();
    APP_CTRL->initAppController();
}

void AppMain::preSetup()
{
    LOGD;
    QFile::remove(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/LDPlayer/Pictures/temp");

    APP_MODEL->setCurrentDir(QDir::currentPath());
    QString checkConnectFilePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/LDPlayer/Applications/" + CHECK_CONNECT_FILENAME;
    if(QFile(checkConnectFilePath).exists() == false){
        QFile file(checkConnectFilePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            LOGD << "Create " << CHECK_CONNECT_FILENAME << "failure";
        }
    }

    QList<QScreen*> listSrc = QGuiApplication::screens();
    if(!listSrc.isEmpty()){
        APP_MODEL->setScreenResolution(QGuiApplication::screens().at(0)->size());
        LOGD << "Screen Size: " << APP_MODEL->screenResolution().width() << "x" << APP_MODEL->screenResolution().height();
    }

    QFile::remove("temp.dat");
}

void AppMain::onLoadConfig()
{
    LOGD;
    if(APP_MODEL->ldIntallFolder() == "")
    {
        LOGD << "LD installation folder has not set yet";
    }else{
        this->initDevicesList();
    }

    QFile configFile(CONFIG_FILE_NAME);
    if(configFile.exists()){
        QJsonObject config = this->loadJson(CONFIG_FILE_NAME).object();

        /* Load Token */
        if(!config[TOKEN_PROP_KEY].toString().isEmpty())
            APP_MODEL->setToken(config[TOKEN_PROP_KEY].toString());

        /* Load installation folder */
#if 0
        if(!config[INSTALL_FOLDER_PROP_KEY].toString().isEmpty())
            APP_MODEL->setLDIntallFolder(config[INSTALL_FOLDER_PROP_KEY].toString(),true);
#endif      
        /* Load AppName */
        if(!config[APP_NAME_PROP_KEY].toString().isEmpty())
            APP_MODEL->setAppName(config[APP_NAME_PROP_KEY].toString());

        /* Load Device count */
        if(!config[DEVICE_COUNT_KEY].isNull())
            APP_MODEL->setDeviceCount(config[DEVICE_COUNT_KEY].toInt());

        /* Load Thread count */
        if(!config[THREAD_COUNT_KEY].isNull())
            APP_MODEL->setAmountOfThread(static_cast<uint>(config[THREAD_COUNT_KEY].toInt()));

    }else{
        this->onSaveConfig();
    }

    QFile version(VERSION_FILENAME);
    if(version.exists()){
        QJsonObject version = this->loadJson(VERSION_FILENAME).object();
        /* Load version */
            APP_MODEL->setVersionCode(version[VERSION_KEY].toInt());
    }
    if(APP_MODEL->versionCode() != APP_MODEL->appConfig().m_cgbconsole_versioncode){
        LOGD << "Current version:" << APP_MODEL->versionCode();
        LOGD << "New version:" << APP_MODEL->appConfig().m_cgbconsole_versioncode;
        updateVersion();
    }
}

void AppMain::onSaveConfig()
{
    LOGD;
    QJsonObject config;
    config[INSTALL_FOLDER_PROP_KEY] = APP_MODEL->ldIntallFolder();
    config[TOKEN_PROP_KEY] = APP_MODEL->token();
    config[APP_NAME_PROP_KEY] = APP_MODEL->appName();
    config[DEVICE_COUNT_KEY] = APP_MODEL->deviceCount();
    config[THREAD_COUNT_KEY] = static_cast<int>(APP_MODEL->amountOfThread());
    this->saveJson(QJsonDocument(config),CONFIG_FILE_NAME);
}

void AppMain::initDevicesList()
{
    LOGD;
    if(APP_MODEL->ldIntallFolder() == ""){
        LOGD << "Installation folder has not set up yet!";
        return;
    }else{
        QString output, error;
        LDCommand::instance()->runLDCommand("list", output, error);
        if(error != ""){
            LOGD << "ERROR: " << error;
        }else{
            QStringList listNameDevices = QString(output).split("\r\n",QString::SkipEmptyParts);
            for (int i = 0; i < listNameDevices.length(); i++) {
                if(listNameDevices.at(i).contains(EMULATOR_NAME_PREFIX)) {
                    APP_MODEL->appendDevice(listNameDevices.at(i));
                }
            }
        }
    }
}

void AppMain::onStartProgram()
{
    LOGD;
    APP_MODEL->setAppStarted(true);
    if(!m_copyDevicesThread.isRunning()){
            m_copyDevicesThread.start();
    }
    this->onSaveConfig();
    if(APP_MODEL->devicesList().length() < APP_MODEL->deviceCount()){
        if(!(LDCommand::instance()->isExistedDevice(ORIGIN_DEVICE_NAME))) {
            this->createTemplateDevice();
        }else {
            // Copy the left devices
            this->copyDevices();
            APP_CTRL->startMultiTask();
        }
    }else {
        APP_CTRL->startMultiTask();
    }

    if(!m_ldServiceThread.isRunning()){
        m_ldServiceThread.start();
    }
    emit startLdService();
}

void AppMain::onStoptProgram()
{
    APP_MODEL->setAppStarted(false);
    APP_CTRL->stopMultiTask();
    emit stopLdService();
}

void AppMain::onFinishCopyDevice(QString deviceName)
{
    LOGD << deviceName;
    APP_MODEL->appendDevice(deviceName);
}

void AppMain::onFinishCreateTemplateDevice()
{
    LOGD;
    if(APP_MODEL->appStarted()){
        this->copyDevices();
        APP_CTRL->startMultiTask();
    }
}

void AppMain::onUpdateFinished(int code)
{
    LOGD << code;
    if(code == AutoUpdaterWorker::E_FINISHED_CODE_NEW_VERSION) {
        APP_MODEL->setIsShowRestartPopup(true);
        QJsonObject config;
        config[VERSION_KEY] = APP_MODEL->appConfig().m_cgbconsole_versioncode;
        this->saveJson(QJsonDocument(config),VERSION_FILENAME);
    } else {
        APP_MODEL->setIsShowRestartPopup(false);
    }
}

void AppMain::onFinishCopyTask()
{
    LOGD;
    if(m_copyInProgress)
        m_copyInProgress = false;
}

void AppMain::closingApp()
{
    LOGD;
    QCoreApplication::quit();
}

int AppMain::restartApplication()
{
    LOGD;
    QProcess::startDetached(QApplication::applicationFilePath());
    exit(12);
}

QJsonDocument AppMain::loadJson(QString fileName)
{
    LOGD << "[AppMain]";
    QFile jsonFile(fileName);
    jsonFile.open(QFile::ReadOnly);
    return QJsonDocument().fromJson(jsonFile.readAll());
}

void AppMain::saveJson(QJsonDocument document, QString fileName)
{
    LOGD << "[AppMain]";
    QFile jsonFile(fileName);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(document.toJson());
}

void AppMain::copyDevices()
{
    LOGD;
    if(m_copyInProgress == false){
	m_copyInProgress = true;
        this->startCopyEmulator();
    }
}

void AppMain::createTemplateDevice()
{
    LOGD;
    this->startCreateTemplateDevice();
}

void AppMain::updateVersion()
{
    LOGD;
    AutoUpdaterWorker* autoUpdateWorker = new AutoUpdaterWorker();
    autoUpdateWorker->moveToThread(&m_updateVersionThread);
    connect(&m_updateVersionThread, &QThread::finished, autoUpdateWorker, &QObject::deleteLater);
    connect(this, &AppMain::startAutoUpdater, autoUpdateWorker, &AutoUpdaterWorker::doWork);
    connect(autoUpdateWorker, &AutoUpdaterWorker::updateFinished, this, &AppMain::onUpdateFinished);
    m_updateVersionThread.start();
    this->startAutoUpdater();
}
