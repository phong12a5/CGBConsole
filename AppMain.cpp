#include "AppMain.h"
#include <QFile>
#include <WebAPI.hpp>
#include <QDir>
#include <QFile>
#include <thread>
#include <QCoreApplication>
#include <QApplication>
#include <AutoUpdaterWorker.h>

#define APP_MODEL   AppModel::instance()
#define APP_CTRL    AppController::instance()
#define WEB_API     WebAPI::instance()

AppMain::AppMain(QObject *parent) : QObject(parent)
{
    connect(APP_MODEL,SIGNAL(reInitDeviceList()),this,SLOT(initDevicesList()));
    connect(APP_MODEL,SIGNAL(sigStartProgram()),this,SLOT(onStartProgram()));
    connect(APP_MODEL,SIGNAL(sigStoptProgram()),this,SLOT(onStoptProgram()));
}

AppMain::~AppMain()
{
    m_copyDevicesThread.quit();
    m_updateVersionThread.quit();
    m_copyDevicesThread.wait();
    m_updateVersionThread.wait();
    LDCommand::instance()->quitAll();
}

void AppMain::initApplication()
{
    LOG;
    APP_MODEL->setCurrentDir(QDir::currentPath());
    this->onLoadConfig();
    APP_CTRL->initAppController();
    m_view = new QQuickView();
    m_view->rootContext()->setContextProperty("AppModel",AppModel::instance());
    m_view->rootContext()->setContextProperty("AppMain",this);
    m_view->setSource(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    m_view->show();
}

void AppMain::onLoadConfig()
{
    LOG;
    if(APP_MODEL->ldIntallFolder() == "")
    {
        LOG << "LD installation folder has not set yet";
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
        LOG << "Current version:" << APP_MODEL->versionCode();
        LOG << "New version:" << APP_MODEL->appConfig().m_cgbconsole_versioncode;
        updateVersion();
    }
}

void AppMain::onSaveConfig()
{
    LOG;
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
    LOG;
    if(APP_MODEL->ldIntallFolder() == ""){
        LOG << "Installation folder has not set up yet!";
        return;
    }else{
        QString output, error;
        LDCommand::instance()->runLDCommand("list", output, error);
        if(error != ""){
            LOG << "ERROR: " << error;
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
    LOG;
    this->onSaveConfig();

    if(APP_MODEL->devicesList().length() < APP_MODEL->deviceCount()){
        if(APP_MODEL->devicesList().isEmpty()) {

            /* --------- Check and download APK --------- */
            LOG << "Downloading APK ...";
            APP_MODEL->setTaskInProgress("Downloading APK ...");
            delay(100);

            QString expectedApkFileName = QString(APK_FILENAME).arg(APP_MODEL->appConfig().m_android_versioncode);
            LOG << "expectedApkFileName: " << expectedApkFileName;
            QDir directory(".");
            QStringList listApks = directory.entryList(QStringList() << "*.apk",QDir::Files);

            if(listApks.contains(expectedApkFileName)) {
                LOG << expectedApkFileName << " is existed already";
            }else {
                if (!WebAPI::instance()->downloadApk(APP_MODEL->appConfig().m_android_versioncode)) {
                    LOG << "Download " << expectedApkFileName << " failure";
                    if(!listApks.isEmpty())
                        expectedApkFileName = listApks.last();
                    else
                        LOG << "Couldn't get any apk file to install!";
                }else {
                    LOG << "Download " << expectedApkFileName << " successfully";
                }
            }

            /* --------- END Check and download APK --------- */

            QString deviceName = ORIGIN_DEVICE_NAME;
            // Create the origin device
            QString output, error;
            LDCommand::instance()->runLDCommand("list", output, error);
            // If ORIGIN_DEVICE has not created, Create it
            if(!output.contains(deviceName)){
                APP_MODEL->setTaskInProgress("Creating the first emulator ...");
                LDCommand::instance()->addInstance(deviceName);
                delay(1000);
            }


            // Install AutoFarmer
            APP_MODEL->setTaskInProgress("Installing APK ...");
            LDCommand::instance()->runLDCommand(QString("installapp --name %1 --filename %2").arg(deviceName).arg(expectedApkFileName));
            while (!LDCommand::instance()->isExistedPackage(deviceName, FARM_PACKAGE_NAME)) {
                delay(1000);
            }

            LDCommand::instance()->ld_adb_command(deviceName,QString("shell mkdir %1").arg(APP_DATA_FOLDER));

            // Disable SuperSU permission request
            LDCommand::instance()->pushFile(deviceName,"/data/data/com.android.settings/databases","./databases");
            LDCommand::instance()->ld_adb_command(deviceName,"shell chown system:system /data/data/com.android.settings/databases/");
            LDCommand::instance()->ld_adb_command(deviceName,"shell chown system:system /data/data/com.android.settings/databases/su*");

            LDCommand::instance()->runLDCommand(QString("modify --name %1 --cpu 1 --memory 1024 --resolution %2").arg(ORIGIN_DEVICE_NAME).arg(APP_MODEL->resolution()));
            LDCommand::instance()->quitInstance(deviceName);
            APP_MODEL->setTaskInProgress("");
        }
        // Copy the left devices
        this->copyDevices();
    }

    APP_CTRL->startMultiTask();
    APP_MODEL->setInitializing(false);
}

void AppMain::onStoptProgram()
{
    APP_CTRL->stopMultiTask();
}

void AppMain::onFinishCopyDevice(QString deviceName)
{
    LOG << deviceName;
    APP_MODEL->appendDevice(deviceName);
}

void AppMain::onUpdateFinished(int code)
{
    LOG << code;
    if(code == AutoUpdaterWorker::E_FINISHED_CODE_NEW_VERSION) {
        APP_MODEL->setIsShowRestartPopup(true);
        QJsonObject config;
        config[VERSION_KEY] = APP_MODEL->appConfig().m_cgbconsole_versioncode;
        this->saveJson(QJsonDocument(config),VERSION_FILENAME);
    } else {
        APP_MODEL->setIsShowRestartPopup(false);
    }
}

void AppMain::closingApp()
{
    LOG;
    QCoreApplication::quit();
}

int AppMain::restartApplication()
{
    LOG;
    QProcess::startDetached(QApplication::applicationFilePath());
    exit(12);
}

QJsonDocument AppMain::loadJson(QString fileName)
{
    LOG << "[AppMain]";
    QFile jsonFile(fileName);
    jsonFile.open(QFile::ReadOnly);
    return QJsonDocument().fromJson(jsonFile.readAll());
}

void AppMain::saveJson(QJsonDocument document, QString fileName)
{
    LOG << "[AppMain]";
    QFile jsonFile(fileName);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(document.toJson());
}

void AppMain::copyDevices()
{
    LOG;
    CopyEmulatorWorker* copyWorker = new CopyEmulatorWorker();
    copyWorker->moveToThread(&m_copyDevicesThread);
    connect(&m_copyDevicesThread, &QThread::finished, copyWorker, &QObject::deleteLater);
    connect(this, &AppMain::startCopyEmulator, copyWorker, &CopyEmulatorWorker::doWork);
    connect(copyWorker, &CopyEmulatorWorker::finishCopyDevice, this, &AppMain::onFinishCopyDevice);
    m_copyDevicesThread.start();
    this->startCopyEmulator();
}

void AppMain::updateVersion()
{
    LOG;
    AutoUpdaterWorker* autoUpdateWorker = new AutoUpdaterWorker();
    autoUpdateWorker->moveToThread(&m_updateVersionThread);
    connect(&m_updateVersionThread, &QThread::finished, autoUpdateWorker, &QObject::deleteLater);
    connect(this, &AppMain::startAutoUpdater, autoUpdateWorker, &AutoUpdaterWorker::doWork);
    connect(autoUpdateWorker, &AutoUpdaterWorker::updateFinished, this, &AppMain::onUpdateFinished);
    m_updateVersionThread.start();
    this->startAutoUpdater();
}
