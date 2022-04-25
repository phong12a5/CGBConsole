#include "LDWorker.h"
#include <AppDefines.h>
#include <LDCommand.h>

LDWorker::LDWorker(QString name, QObject *parent) :
    QObject(parent),
    m_name(name)
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
}
