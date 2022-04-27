#include "AppController.h"
#include <PerformanceReader.h>

#define APP_MODEL AppModel::instance()

AppController* AppController::m_instance = nullptr;

AppController::AppController(QObject *parent) : QObject(parent)
{
    m_ldServiceMap.clear();
    m_ldObserver = nullptr;
}

AppController *AppController::instance()
{
    if(m_instance == nullptr)
        m_instance = new AppController();
    return m_instance;
}

void AppController::initAppController()
{
    LOGD("");
}

void AppController::startLDPlayers()
{
    if(m_ldObserver == nullptr) {
        m_ldObserver = new QTimer(this);
        m_ldObserver->setInterval(3000);
        m_ldObserver->setSingleShot(false);
        connect(m_ldObserver, &QTimer::timeout, this, &AppController::onCheckLDServices);
    }

    if(!m_ldObserver->isActive())
        m_ldObserver->start();
}

void AppController::stopLDPlayers()
{
    if(m_ldObserver && !m_ldObserver->isActive()) {
        m_ldObserver->stop();
    }

    foreach(int serviceID, m_ldServiceMap.keys()) {
        LDService* service = m_ldServiceMap.value(serviceID);
        if(service != nullptr) service->disposeService();
    }
}

LDService* AppController::createLDService()
{
    LDService* service = new LDService(m_ldServiceMap.keys().length(), this);
    connect(service, &LDService::statusChanged, this, &AppController::onLDServiceUpdate);
    m_ldServiceMap.insert(m_ldServiceMap.keys().length(), service);
    return service;
}

void AppController::onLDServiceUpdate(int serviceId)
{
    if(m_ldServiceMap.contains(serviceId)) {
        LDService* service = m_ldServiceMap.value(serviceId);
        LDService::E_SERVICE_STATUS status = service->serviceStatus();
        LOGD("service: " + QString::number(serviceId) + " status: " + QString::number(status));
        if(status == LDService::E_SERVICE_STATUS_STOPPED) {
            m_ldServiceMap.remove(serviceId);
            delete service;
        }
    }

    if(m_ldServiceMap.empty()) LDCommand::quitAll();
}

void AppController::onCheckLDServices()
{
    foreach(int ldInsId, m_ldServiceMap.keys()) {
        LDService* service = m_ldServiceMap.value(ldInsId);
        QWidget* widget = service->getLDWidget();
        if(widget) {
            APP_MODEL->updateDevicesWidgetMap(ldInsId, widget);
        }
    }

    double cpuPercent = PerformanceReader::instance()->currentCPUUsage();
    double diskPercent = PerformanceReader::instance()->currentDiskUsage();
    LOGD(QString("cpuPercent: %1 -- diskPercent: %2").arg(cpuPercent).arg(diskPercent));
    if(cpuPercent > AVAILBLE_CPU_USAGE ) return;

    if(APP_MODEL->appStarted()) {
        if(m_ldServiceMap.keys().count() < AppModel::instance()->maxNumberThread()) {
            LDService* service = createLDService();
            service->startService();
        }
    }
}
