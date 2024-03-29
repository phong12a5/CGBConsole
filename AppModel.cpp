#include "AppModel.h"
#include "AppMain.h"
#include "WebAPI.hpp"
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
    m_deviceCount = DEFAULT_MAX_VM_COUNT;
    m_maxNumberThread = DEFAULT_MAX_VM_THREAD;
    m_maxVMCount = DEFAULT_MAX_VM_COUNT;
    m_taskInProgress = "";
    m_resolution = "720,1280,320";
    m_isShowRestartPopup = false;
    m_versionCode = 0;
    m_appStarted = false;
    m_serialNumber = false;
    m_testMode = false;
    m_validToken = false;
    m_cpuCoreCount = PerformanceReader::instance()->cpuCoreCount();

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

bool AppModel::appStarted() const
{
    return m_appStarted;
}

void AppModel::setAppStarted(bool data)
{
    if(m_appStarted != data) {
        m_appStarted = data;
        emit appStartedChanged();
    }
}

QString AppModel::ldIntallFolder() const
{
    return m_ldIntallFolder;
}

void AppModel::setLDIntallFolder(const QString path, bool standardPath)
{
    QString tmp_path = path;
    if(m_ldIntallFolder != "") {
        LOGD("There is a valid Path already!");
        return;
    }
    if(standardPath == false)
        tmp_path = path.mid(8);
    else
        tmp_path = path;

    LOGD("path: " + tmp_path);

    if(QFile(tmp_path + "/ldconsole.exe").exists()){
        if(m_ldIntallFolder != tmp_path ){
            m_ldIntallFolder = tmp_path;
            emit ldIntallFolderChanged();
        }
    }else{
        LOGD("Invalid folder.");
        return;
    }

    emit reInitDeviceList();
}

QList<QObject*> AppModel::devicesList() const
{
    return m_devicesList;
}

void AppModel::appendDevice(QString instanceName)
{
    foreach(QObject* device, m_devicesList) {
        if(dynamic_cast<LDIntance*>(device)->instanceName() == instanceName)
            return;
    }
    m_devicesList.append(new LDIntance(this,instanceName,m_devicesList.length()));
    emit devicesListChanged();
}

QList<QObject *> AppModel::devicesRunningList() const
{
    return m_devicesRunningList;
}

void AppModel::appendRunningDevice(LDIntance* instance)
{
    LOGD(instance->instanceName());
    m_devicesRunningList.append(instance);
    emit devicesRunningListChanged();
}

void AppModel::popRunningDevice(LDIntance* instance)
{
    LOGD(instance->instanceName());
    m_devicesRunningList.removeOne(instance);
    emit devicesRunningListChanged();
}

uint AppModel::amountOfThread() const
{
    return m_amountOfThread;
}

void AppModel::setAmountOfThread(const uint data)
{
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
    LOGD(data);
    if(m_token != data ){
        m_token = data;
        if(m_token.length() == 32 && WebAPI::instance()->getConfig() == true){
            this->setValidToken(true);
        }else {
            this->setValidToken(false);
        }
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
//    this->setWalletEmpty(m_appConfig.m_balance >= 0 && m_appConfig.m_balance <= 50);
    this->setMaxVMCount(m_appConfig.m_maxVmCount);
    this->setMaxNumberThread(m_appConfig.m_maxVmThread);
}

int AppModel::maxNumberThread() const
{
    return m_maxNumberThread;
}

void AppModel::setMaxNumberThread(int data)
{
    LOGD(QString::number(data));
    if(m_maxNumberThread != data) {
        m_maxNumberThread = data;
        emit maxNumberThreadChanged();
    }
}

int AppModel::maxVMCount() const
{
    return m_maxVMCount;
}

void AppModel::setMaxVMCount(int data)
{
    LOGD(QString::number(data));
    if(m_maxVMCount != data) {
        m_maxVMCount = data;
        emit maxVMCountChanged();
    }
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
}

QString AppModel::appName() const
{
    return m_appName;
}

void AppModel::setAppName(QString data)
{
    LOGD(data);
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
    LOGD(QString::number(data));
    if(m_deviceCount != data) {
        m_deviceCount = data;
        emit deviceCountChanged();
    }
}

QString AppModel::taskInProgress() const
{
    return m_taskInProgress;
}

void AppModel::setTaskInProgress(QString data)
{
    if(m_taskInProgress != data) {
        m_taskInProgress = data;
        emit taskInProgressChanged();
    }
}

QString AppModel::resolution() const
{
    return m_resolution;
}

void AppModel::setResolution(QString data)
{
    LOGD(data);
    if (m_resolution != data) {
        m_resolution = data;
        emit resolutionChanged();;
    }
}

bool AppModel::isShowRestartPopup() const
{
    return m_isShowRestartPopup;
}

void AppModel::setIsShowRestartPopup(bool data)
{
    if(m_isShowRestartPopup != data)
    {
        m_isShowRestartPopup = data;
        emit isShowRestartPopupChanged();
    }
}

int AppModel::versionCode() const
{
    return m_versionCode;
}

void AppModel::setVersionCode(int data)
{
    if( m_versionCode != data) {
        m_versionCode = data;
        emit versionCodeChanged();
    }
}

QSize AppModel::screenResolution() const
{
    return m_screenResolution;
}

void AppModel::setScreenResolution(const QSize &data)
{
    if(data != m_screenResolution){
        m_screenResolution = data;
    }
}

QString AppModel::serialNumber() const
{
    return m_serialNumber;
}

void AppModel::setSerialNumber(QString data)
{
    LOGD(data);
    if(m_serialNumber != data){
        m_serialNumber = data;
    }
}

bool AppModel::testMode() const
{
    return m_testMode;
}

void AppModel::setTestMode(bool data)
{
    if(m_testMode != data){
        m_testMode = data;
        emit testModeChanged();
    }
}

bool AppModel::validToken() const
{
    return m_validToken;
}

void AppModel::setValidToken(bool data)
{
    if(m_validToken != data){
        m_validToken = data;
        emit validTokenChanged();
    }
}

int AppModel::cpuCoreCount() const
{
    return m_cpuCoreCount;
}

void AppModel::startProgram()
{
    emit sigStartProgram();
}

void AppModel::stopProgarm()
{
    emit sigStoptProgram();
}





