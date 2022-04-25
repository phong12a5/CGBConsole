#include "LDService.h"
#include <AppModel.h>
#include <QDir>
#include <QStandardPaths>
#include <QString>
#include <AppModel.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <LDWorker.h>

LDService::LDService(int serviceId, QObject *parent) :
    QObject(parent),
    m_serviceId(serviceId),
    m_serviceStatus(E_SERVICE_STATUS_INITIALIZING)
{
    m_ldThread = new QThread();
    m_ldWorker = new LDWorker("LDPlayer-" + QString::number(m_serviceId));

    connect(m_ldThread, &QThread::started, this, &LDService::onThreadStarted);
    connect(m_ldThread, &QThread::finished, this, &LDService::onThreadFinished);

    m_ldWorker->moveToThread(m_ldThread);
    LOGD(QString("--- Create LDService:%1---").arg(m_serviceId));
}

LDService::~LDService()
{
    LOGD(QString(":%1").arg(m_serviceId));
    if(m_ldThread) {
        m_ldThread->quit();
        m_ldThread->wait();
        delete m_ldThread;
    }

    if(m_ldWorker) {
        delete m_ldWorker;
    }
}

LDService::E_SERVICE_STATUS LDService::serviceStatus() const
{
    return m_serviceStatus;
}

void LDService::setServiceStatus(E_SERVICE_STATUS status)
{
    if(m_serviceStatus != status) {
        m_serviceStatus = status;
        emit statusChanged(m_serviceId);
    }
}

void LDService::startService()
{
    LOGD("");
    if(!m_ldThread->isRunning())
        m_ldThread->start();
}

void LDService::disposeService()
{
    LOGD("");
    if(m_ldThread->isRunning()) {
        m_ldThread->quit();
    } else {
        setServiceStatus(E_SERVICE_STATUS_STOPPED);
    }
}


void LDService::onThreadStarted()
{
    LOGD(QString::number(m_serviceId));
    emit m_ldWorker->start();
    setServiceStatus(E_SERVICE_STATUS_RUNNING);
}

void LDService::onThreadFinished()
{
    LOGD(QString::number(m_serviceId));
    setServiceStatus(E_SERVICE_STATUS_STOPPED);
}
