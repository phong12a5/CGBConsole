#include "AppController.h"
#include "LDIntance.h"

#define APP_MODEL AppModel::instance()

AppController* AppController::m_instance = nullptr;

AppController::AppController(QObject *parent) : QObject(parent)
{
    m_deviceQueue.clear();
    m_ldThreadList.clear();
    m_updateLDThreadList.setInterval(5000);
    m_updateLDThreadList.setSingleShot(false);
    connect(&m_updateLDThreadList, SIGNAL(timeout()), this, SLOT(onUpdateLDThreadList()));
}

AppController *AppController::instance()
{
    if(m_instance == nullptr)
        m_instance = new AppController();
    return m_instance;
}

void AppController::initAppController()
{
    LOGD;
}

void AppController::startMultiTask()
{
    LOGD;
    this->onUpdateLDThreadList();
    m_updateLDThreadList.start();
}

void AppController::stopMultiTask()
{
    LOGD;
    LDCommand::instance()->quitAll();
    while (!m_ldThreadList.isEmpty()) {
        delete m_ldThreadList.at(0);
        m_ldThreadList.removeAt(0);
    }
    m_deviceQueue.clear();
    LDCommand::instance()->quitAll();
    m_updateLDThreadList.stop();
}

void AppController::aMissionCompleted(LDThread* threadAdd)
{
    if(threadAdd){
        if(m_ldThreadList.contains(threadAdd)){
            m_ldThreadList.removeOne(threadAdd);
            delete threadAdd;
        }
    }
}

void AppController::onUpdateLDThreadList()
{
    double diskUsage = PerformanceReader::currentDiskUsage();
    if(diskUsage < 0 || diskUsage > AVAILBLE_DISK_USAGE ){
        LOGD << "Disk usage is too large ... NONE NEW DEVICE IS STARTED!";
        return;
    }

    double curCPU = PerformanceReader::currentCPUUsage();
    if(curCPU < 0 || curCPU > AVAILBLE_CPU_USAGE ){
        LOGD << "CPU usage is too large ... NONE NEW DEVICE IS STARTED!";
        return;
    }

    /* ******** Update m_deviceQueue from devicesList ******** */
    foreach (QObject* device, APP_MODEL->devicesList()) {
        if(!m_deviceQueue.contains(device))
            m_deviceQueue.append(device);
    }

    if(static_cast<uint>(m_ldThreadList.length()) < APP_MODEL->amountOfThread()){
        if(m_deviceQueue.isEmpty()){
            LOGD << "m_deviceQueue is empty!";
        } else {
            foreach (QObject* device, m_deviceQueue) {
                if(!APP_MODEL->devicesRunningList().contains(device)){
                    m_ldThreadList.append(new LDThread(this,dynamic_cast<LDIntance*>(device)));
                    m_deviceQueue.removeOne(device);
                    m_deviceQueue.append(device);
                    break;
                } else {
                    LOGD << dynamic_cast<LDIntance*>(device)->instanceName() << " is run already";
                }
            }
        }
    }else {
        LOGD << "LDThreadList is full already!";
    }
}
