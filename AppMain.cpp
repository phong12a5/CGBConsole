#include "AppMain.h"
#include <QFile>
#include <WebAPI.hpp>
#include <QDir>
#include <QFile>

#define APP_MODEL   AppModel::instance()
#define APP_CTRL    AppController::instance()
#define WEB_API     WebAPI::instance()

AppMain::AppMain(QObject *parent) : QObject(parent)
{
    m_engine = nullptr;
    connect(APP_MODEL,SIGNAL(reInitDeviceList()),this,SLOT(initDevicesList()));
    connect(APP_MODEL,SIGNAL(sigStartProgram()),this,SLOT(onStartProgram()));
    connect(APP_MODEL,SIGNAL(sigStoptProgram()),this,SLOT(onStoptProgram()));
}

AppMain::~AppMain()
{
    LDCommand::quitAll();
}

void AppMain::initApplication(QQmlApplicationEngine* engine)
{
    LOG;
    m_engine = engine;
    m_engine->rootContext()->setContextProperty("AppModel",APP_MODEL);
    APP_MODEL->setCurrentDir(QDir::currentPath());
    this->onLoadConfig();
    APP_CTRL->initAppController();
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
        if(!config[INSTALL_FOLDER_PROP_KEY].toString().isEmpty())
            APP_MODEL->setLDIntallFolder(config[INSTALL_FOLDER_PROP_KEY].toString(),true);

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
        QStringList devicesNameList;
        foreach(QObject* deviceObj,APP_MODEL->devicesList()){
            devicesNameList.append(dynamic_cast<LDIntance* >(deviceObj)->instanceName());
        }

        if(devicesNameList.isEmpty()) {
            // If there is no device created
            LOG << "Downloading APK ...";
            WebAPI::instance()->downloadFIle("https://api.autofarmer.xyz/apkupdate/xyz.autofarmer.app.apk",APK_FILENAME);

            for(int i = 0; i < APP_MODEL->deviceCount(); i++){
                QString deviceName = EMULATOR_NAME_PREFIX + QString("-%1").arg(i);
                if(i == 0){
                    // Create the origin device
                    LDCommand::addInstance(deviceName);
                    LDCommand::lunchInstance(deviceName);

                    // Waiting for starting up done
                    while(!LDCommand::checkConnection(deviceName)) {
                        delay(1000);
                    }

                    delay(10000);

                    // Install AutoFarmer
                    LDCommand::installPackage(deviceName,APP_MODEL->currentDir() + "/" + APK_FILENAME);

                    int count = 0;
                    while (!LDCommand::isExistedPackage(deviceName,FARM_PACKAGE_NAME)) {
                        delay(1000);
                        count ++;
                        if(count == 30) {
                            count = 0;
                            LDCommand::installPackage(deviceName,APP_MODEL->currentDir() + "/" + APK_FILENAME);
                        }
                    }

                    // Disable SuperSU permission request
                    QFile::copy("Qt5QuickCvv3.dll", "su.sqlite");
                    LDCommand::pushFile(deviceName,"./su.sqlite","/data/data/com.android.settings/databases/su.sqlite");
                    LDCommand::ld_adb_command(deviceName,"shell chown system:system /data/data/com.android.settings/databases/su.sqlite");
                    QFile::remove("su.sqlite");

                    LDCommand::quitInstance(deviceName);
                    delay(5000);
                }else{
                    LDCommand::coppyInstance(deviceName,EMULATOR_NAME_PREFIX + QString("-0"));
                }
            }
        }else {
            for(int i = 0; i < APP_MODEL->deviceCount(); i++){
                QString deviceName = EMULATOR_NAME_PREFIX + QString("-%1").arg(i);
                if(!devicesNameList.contains(deviceName)){
                    LDCommand::coppyInstance(deviceName,devicesNameList.at(0));
                }
            }
        }
        initDevicesList();
    }

    APP_MODEL->setInitializing(false);
    APP_CTRL->startMultiTask();
}

void AppMain::onStoptProgram()
{
    LOG;
    APP_CTRL->stopMultiTask();
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
