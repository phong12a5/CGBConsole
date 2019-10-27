#include "AppMain.h"
#include <QFile>
#include <WebAPI.hpp>
#include <QDir>
#include <QFile>
#include <thread>
#include <QCoreApplication>

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
    m_copyDevicesThread.wait();
    LDCommand::quitAll();
}

void AppMain::initApplication()
{
    LOG;
    m_view = new QQuickView();
    m_view->rootContext()->setContextProperty("AppModel",AppModel::instance());
    m_view->rootContext()->setContextProperty("AppMain",this);
    m_view->setSource(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    m_view->show();

    APP_MODEL->setTaskInProgress("Initializing application ...");
    APP_MODEL->setCurrentDir(QDir::currentPath());
    this->onLoadConfig();
    APP_CTRL->initAppController();
    APP_MODEL->setTaskInProgress("");
}

void AppMain::getConfig()
{
    WEB_API->getConfig();
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

        /* Load installation folder */
#if 0
        if(!config[INSTALL_FOLDER_PROP_KEY].toString().isEmpty())
            APP_MODEL->setLDIntallFolder(config[INSTALL_FOLDER_PROP_KEY].toString(),true);
#endif

        /* Load Token */
        if(!config[TOKEN_PROP_KEY].toString().isEmpty())
            APP_MODEL->setToken(config[TOKEN_PROP_KEY].toString());

        /* Load AppName */
        if(!config[APP_NAME_PROP_KEY].toString().isEmpty())
            APP_MODEL->setAppName(config[APP_NAME_PROP_KEY].toString());

        /* Load Device count */
        if(!config[DEVICE_COUNT_KEY].isNull())
            APP_MODEL->setDeviceCount(config[DEVICE_COUNT_KEY].toInt());
    }else{
        this->onSaveConfig();
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
        LDCommand::runLDCommand("list", output, error);
        if(error != ""){
            LOG << "ERROR: " << error;
        }else{
            QStringList listNameDevices = QString(output).split("\r\n",QString::SkipEmptyParts);
            QList<QObject*> deviceList;
            deviceList.clear();
            for (int i = 0; i < listNameDevices.length(); i++) {
                if(listNameDevices.at(i).contains(EMULATOR_NAME_PREFIX)) {
                    deviceList.append(new LDIntance(this,listNameDevices.at(i)));
                    LOG << "Added " << listNameDevices.at(i);
                }
            }
            APP_MODEL->setDevicesList(deviceList);
        }
    }
}

void AppMain::onStartProgram()
{
    LOG;
    this->onSaveConfig();
    WEB_API->getConfig();

    if(APP_MODEL->devicesList().length() < APP_MODEL->deviceCount()){
        if(APP_MODEL->devicesList().isEmpty()) {
            // If there is no device created
            LOG << "Downloading APK ...";
            APP_MODEL->setTaskInProgress("Downloading APK ...");
            WebAPI::instance()->downloadFIle("https://api.autofarmer.xyz/apkupdate/xyz.autofarmer.app.apk",APK_FILENAME);
            APP_MODEL->setTaskInProgress("Creating the first emulator ...");

            QString deviceName = ORIGIN_DEVICE_NAME;
            // Create the origin device
            QString output, error;
            LDCommand::runLDCommand("list", output, error);
            // If ORIGIN_DEVICE has not created, Create it
            if(!output.contains(deviceName))
                LDCommand::addInstance(deviceName);

            LDCommand::lunchInstance(deviceName);

            // Waiting for starting up done
            while(!LDCommand::checkConnection(deviceName)) {
                delay(1000);
            }

            // Install AutoFarmer
            APP_MODEL->setTaskInProgress("Installing APK ...");
            while (!LDCommand::isExistedPackage(deviceName,FARM_PACKAGE_NAME)) {
                LDCommand::installPackage(deviceName,APK_FILENAME,APP_MODEL->currentDir() + "/" + APK_FILENAME);
            }

            // Disable SuperSU permission request
            QFile::copy("Qt5QuickCvv3.dll", "su.sqlite");
            LDCommand::pushFile(deviceName,"./su.sqlite","/data/data/com.android.settings/databases/su.sqlite");
            LDCommand::ld_adb_command(deviceName,"shell chown system:system /data/data/com.android.settings/databases/su.sqlite");
            QFile::remove("su.sqlite");

            QFile::copy("Qt5QuickCvv4.dll", "supersuer.sqlite");
            LDCommand::pushFile(deviceName,"./supersuer.sqlite","/data/data/com.android.settings/databases/supersuer.sqlite");
            LDCommand::ld_adb_command(deviceName,"shell chown system:system /data/data/com.android.settings/databases/supersuer.sqlite");
            QFile::remove("supersuer.sqlite");

            LDCommand::runLDCommand(QString("modify --name %1 --cpu 1 --memory 1024 --resolution 720,1280,320").arg(ORIGIN_DEVICE_NAME));
            LDCommand::quitInstance(deviceName);
            APP_MODEL->setTaskInProgress("");
        }
        // Copy the left devices
        this->copyDevices();
    }


    APP_MODEL->setInitializing(false);
    APP_CTRL->startMultiTask();
}

void AppMain::onStoptProgram()
{
    LOG;
    APP_CTRL->stopMultiTask();
}

void AppMain::onFinishCopyDevice(QString deviceName)
{
    LOG << deviceName;
    APP_MODEL->appendDevice(new LDIntance(this,deviceName));
    APP_CTRL->startMultiTask();
}

void AppMain::closingApp()
{
    LOG;
    QCoreApplication::quit();
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
