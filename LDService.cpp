#include "LDService.h"
#include <AppModel.h>
#include <QDir>
#include <QStandardPaths>
#include <QString>
#include <AppModel.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <LDWorker.h>
#include <QWindow>
#include <QWidget>

LDService::LDService(int serviceId, QObject *parent) :
    QObject(parent),
    m_serviceId(serviceId),
    m_ldInsName("LDPlayer-" + QString::number(m_serviceId)),
    m_serviceStatus(E_SERVICE_STATUS_INITIALIZING),
    m_ldWindow(nullptr),
    m_ldWidget(nullptr),
    m_ldTopWindow(nullptr),
    m_ldTopWidget(nullptr)
{
    m_ldThread = new QThread();
    m_ldWorker = new LDWorker(m_serviceId);

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

    if(m_ldWidget){
        m_ldWidget->close();
    }

    if(m_ldWindow) {
        m_ldWindow->close();
        m_ldWindow->destroy();
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
    LOGD(m_ldInsName);
    if(!m_ldThread->isRunning())
        m_ldThread->start();
}

QWidget* LDService::getLDWidget()
{
//    if(m_ldWidget == nullptr) {
//        int bindWinId = LDCommand::bindWinId(m_ldInsName);
//        if(bindWinId > 0) {
//            m_ldWindow = QWindow::fromWinId(bindWinId);
//            m_ldWidget  = QWidget::createWindowContainer(m_ldWindow);
//        }
//    }

//    if(m_ldTopWidget == nullptr) {
//        int topWinId = LDCommand::topWinId(m_ldInsName);
//        LOGD(topWinId);
//        if(topWinId > 0) {
//            m_ldTopWindow = QWindow::fromWinId(topWinId);
//            m_ldTopWidget  = QWidget::createWindowContainer(m_ldTopWindow);
//        }
//        if(m_ldTopWidget) {
//            m_ldTopWindow->close();
//            m_ldTopWidget->close();
//        }
//    }

    return m_ldWidget;
}

void LDService::disposeService()
{
    LOGD(m_ldInsName);
    if(m_ldThread->isRunning()) {
        m_ldWorker->stop();
        m_ldThread->quit();
    } else {
        setServiceStatus(E_SERVICE_STATUS_STOPPED);
    }
}


void LDService::onThreadStarted()
{
    LOGD(m_ldInsName);
    m_ldWorker->start();
    setServiceStatus(E_SERVICE_STATUS_RUNNING);
}

void LDService::onThreadFinished()
{
    LOGD(QString::number(m_serviceId));
    setServiceStatus(E_SERVICE_STATUS_STOPPED);
}

void LDService::onWorkerStarted()
{
    LOGD(m_ldInsName);
}

void LDService::onWorkerStopped()
{
    LOGD(m_ldInsName);
}
