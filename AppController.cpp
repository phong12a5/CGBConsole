#include "AppController.h"

#define APP_MODEL AppModel::instance()

AppController* AppController::m_instance = nullptr;

AppController::AppController(QObject *parent) : QObject(parent)
{
    m_ldThreadList.clear();
}

AppController *AppController::instance()
{
    if(m_instance == nullptr)
        m_instance = new AppController();
    return m_instance;
}

void AppController::initAppController()
{
    QObject::connect(APP_MODEL,SIGNAL(devicesListChanged()),this,SLOT(onDevicesListChanged()));
}

void AppController::startMultiTask()
{
    LOG;
    int amountOfRunThread = 0;
    if(APP_MODEL->devicesList().length() > static_cast<int>(APP_MODEL->amountOfThread()))
        amountOfRunThread = static_cast<int>(APP_MODEL->amountOfThread());
    else {
        amountOfRunThread = APP_MODEL->devicesList().length();
    }

    while (m_ldThreadList.length() < amountOfRunThread) {
        m_ldThreadList.append(new LDThread(this));
    }
}

void AppController::stopMultiTask()
{
    LOG;
    while (!m_ldThreadList.isEmpty()) {
        delete m_ldThreadList.at(0);
        m_ldThreadList.removeAt(0);
    }
}

void AppController::startANewDevice()
{

}

void AppController::onDevicesListChanged()
{
    LOG;
}

void AppController::aMissionCompleted(LDThread* threadAdd)
{
    if(threadAdd){
        if(m_ldThreadList.contains(threadAdd)){
            int index = m_ldThreadList.indexOf(threadAdd);
            delete m_ldThreadList.at(index);
            m_ldThreadList.removeAt(index);
        }
        this->startMultiTask();
    }
}
