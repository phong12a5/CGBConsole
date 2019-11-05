#include "LDThread.h"
#include "AppModel.h"
#include "AppController.h"

#define APP_CTRL    AppController::instance()
#define APP_MODEL   AppModel::instance()

LDThread::LDThread(QObject *parent, LDIntance* ldInstance) :
    QObject(parent)
{
    m_Worker = nullptr;
    m_ldInstance = ldInstance;
    APP_MODEL->appendRunningDevice(m_ldInstance->instanceName());

    connect(this, SIGNAL(missionCompleted(LDThread*)),APP_CTRL, SLOT(aMissionCompleted(LDThread*)));

    m_Worker = new LDRunner(m_ldInstance->instanceName());
    m_Worker->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::finished, m_Worker, &LDRunner::deleteLater);
    connect(this, &LDThread::operate, m_Worker, &LDRunner::run);
    connect(this, &LDThread::quitThread, m_Worker, &LDRunner::quitRunner);
    connect(m_Worker, &LDRunner::finished, this, &LDThread::finishedATask);
    m_workerThread.start();
    emit this->operate();
}

LDThread::~LDThread()
{
    APP_MODEL->popRunningDevice(m_ldInstance->instanceName());
    emit quitThread();
    m_workerThread.quit();
    m_workerThread.wait();
}

void LDThread::finishedATask()
{
    emit missionCompleted(this);
}
