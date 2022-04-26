#include "LDWorker.h"
#include <AppDefines.h>
#include <LDCommand.h>
#include <QMutex>

LDWorker::LDWorker(QString name, QObject *parent) :
    QObject(parent),
    m_name(name),
    m_started(false)
{
    connect(this, &LDWorker::post, this, &LDWorker::run);
}

LDWorker::~LDWorker()
{
    LOGD(m_name);
}

void LDWorker::start()
{
    LOGD(m_name);
    QMutex mutex;
    mutex.lock();
    m_started = true;
    LDCommand::instance()->lunchInstance(m_name);
    postDelay(0);
    mutex.unlock();
}

void LDWorker::stop()
{
    LOGD(m_name);
    QMutex mutex;
    mutex.lock();
    m_started = false;
    LDCommand::instance()->quitInstance(m_name);
    mutex.unlock();
}

void LDWorker::run()
{
    if(!m_started) return;

    if(!app_running) {
        LDCommand::instance()->runApp(m_name, "com.facebook.katana");
        app_running = true;
    } else {
        LDCommand::instance()->killApp(m_name, "com.facebook.katana");
        app_running = false;
    }

    postDelay(7000);
}

void LDWorker::postDelay(int milSec)
{
    if(milSec > 0)
        delay(milSec);

    emit post();
}
