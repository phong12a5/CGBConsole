#include "LDWorker.h"
#include <AppDefines.h>
#include <LDCommand.h>

LDWorker::LDWorker(QString name, QObject *parent) :
    QObject(parent),
    m_name(name),
    m_appChecker(nullptr)
{
    connect(this, &LDWorker::start, this, &LDWorker::onStarted);
}

LDWorker::~LDWorker()
{
    LOGD(m_name);
    LDCommand::instance()->quitInstance(m_name);
}

void LDWorker::onStarted()
{
    LOGD(m_name);
    LDCommand::instance()->lunchInstance(m_name);
    if(m_appChecker == nullptr) {
        m_appChecker = new QTimer(this);
        m_appChecker->setInterval(7000);
        connect(m_appChecker, &QTimer::timeout, this, &LDWorker::onCheckRunningApp);
    }
    if(!m_appChecker->isActive()) {
        m_appChecker->start();
    }
}

void LDWorker::onCheckRunningApp()
{
    if(!app_running) {
        LDCommand::instance()->runApp(m_name, "com.facebook.katana");
        app_running = true;
    } else {
        LDCommand::instance()->killApp(m_name, "com.facebook.katana");
        app_running = false;
    }
}
