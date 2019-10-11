#include "AppModel.h"
#include "AppMain.h"
#include <QFile>
#include <QDir>

AppModel* AppModel::m_instance = nullptr;

AppModel::AppModel(QObject *parent) : QObject(parent)
{
    m_amountOfThread = 1;
    m_isLaunchMutiTask = false;
    m_latestRunningInstance = -1;
    m_token = "";
    m_initializing = false;
    m_currentDir = "";
    m_appName = "fblite";
    m_walletEmpty = false;
    m_deviceCount = 30;

    if(QFile(QDir::currentPath() + QString("/LDSetup/ldconsole.exe")).exists()){
        m_ldIntallFolder = QDir::currentPath() + QString("/LDSetup");
    }else {
        m_ldIntallFolder = "";
    }
}

AppModel *AppModel::instance()
{
    if(m_instance == nullptr){
        m_instance = new AppModel();
    }
    return m_instance;
}

QString AppModel::ldIntallFolder() const
{
    return m_ldIntallFolder;
}

void AppModel::setLDIntallFolder(const QString path, bool standardPath)
{
    QString tmp_path = path;
    if(m_ldIntallFolder != "") {
        LOG << "There is a valid Path already!";
        return;
    }
    if(standardPath == false)
        tmp_path = path.mid(8);
    else
        tmp_path = path;

    LOG << "path: " << tmp_path;

    if(QFile(tmp_path + "/ldconsole.exe").exists()){
        if(m_ldIntallFolder != tmp_path ){
            m_ldIntallFolder = tmp_path;
            emit ldIntallFolderChanged();
        }
    }else{
        LOG << "Invalid folder.";
        return;
    }

    emit reInitDeviceList();
}

QList<QObject*> AppModel::devicesList() const
{
    return m_devicesList;
}

void AppModel::setDevicesList(QList<QObject*> devices)
{
    if(m_devicesList != devices){
        m_devicesList = devices;
        emit devicesListChanged();
    }
}

void AppModel::appendDevice(QObject* instance)
{
    foreach(QObject* device, m_devicesList) {
        if(dynamic_cast<LDIntance*>(device)->instanceName() == dynamic_cast<LDIntance*>(instance)->instanceName())
            return;
    }
    m_devicesList.append(instance);
    LOG << "m_devicesList: " << m_devicesList.length();
    emit devicesListChanged();
}

uint AppModel::amountOfThread() const
{
    return m_amountOfThread;
}

void AppModel::setAmountOfThread(const uint data)
{
    LOG << data;
    if(data != m_amountOfThread){
        m_amountOfThread = data;
        emit amountOfThreadChanged();
    }
}

bool AppModel::isLaunchMutiTask() const
{
    return m_isLaunchMutiTask;
}

void AppModel::setIsLaunchMutiTask(const bool data)
{
    LOG << data;
    if(m_isLaunchMutiTask != data){
        m_isLaunchMutiTask = data;
        emit isLaunchMutiTaskChanged();
    }
}

int AppModel::latestRunningInstance() const
{
    return m_latestRunningInstance;
}

void AppModel::setLatestRunningInstance(const int data)
{
    if(m_latestRunningInstance != data){
        QMutex mutex;
        mutex.lock();
        if(data >= this->devicesList().length()){
            m_latestRunningInstance = 0;
        }else{
            m_latestRunningInstance = data;
        }
        mutex.unlock();
    }
}

QString AppModel::token() const
{
    return m_token;
}

void AppModel::setToken(QString data)
{
    LOG << data;
    if(m_token != data ){
        m_token = data;
        emit tokenChanged();
    }
}

APP_CONFIG AppModel::appConfig() const
{
    return m_appConfig;
}

void AppModel::setAppConfig(APP_CONFIG data)
{
    m_appConfig = data;
    this->setWalletEmpty(m_appConfig.m_balance >= 0 && m_appConfig.m_balance <= 50);
#ifdef TEST_MODE
    m_appConfig.m_ldCount = 30;
#endif
}

int AppModel::maxNumberThread() const
{
    return MAX_THREAD;
}

bool AppModel::initializing() const
{
    return m_initializing;
}

void AppModel::setInitializing(bool data)
{
    if(m_initializing != data){
        m_initializing = data;
        emit initializingChanged();
    }
}

QString AppModel::currentDir() const
{
    return m_currentDir;
}

void AppModel::setCurrentDir(QString data)
{
    m_currentDir = data;
    LOG << m_currentDir;
}

QString AppModel::appName() const
{
    return m_appName;
}

void AppModel::setAppName(QString data)
{
    if(m_appName != data){
        m_appName = data;
        emit appNameChanged();
    }
}

bool AppModel::walletEmpty() const
{
    return m_walletEmpty;
}

void AppModel::setWalletEmpty(bool data)
{
    if(data != m_walletEmpty) {
        m_walletEmpty = data;
        emit walletEmptyChanged();
    }
}

int AppModel::deviceCount() const
{
    return m_deviceCount;
}

void AppModel::setDeviceCount(int data)
{
    LOG << data;
    if(m_deviceCount != data) {
        m_deviceCount = data;
        emit deviceCountChanged();
    }
}

void AppModel::startProgram()
{
    emit sigStartProgram();
}

void AppModel::stopProgarm()
{
    emit sigStoptProgram();
}





