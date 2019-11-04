#include "LDThread.h"
#include "AppModel.h"
#include "AppController.h"

#define APP_CTRL    AppController::instance()
#define APP_MODEL   AppModel::instance()

LDThread::LDThread(QObject *parent, QString ldInstanceName) :
    QObject(parent),
    m_ldInstanceName(ldInstanceName)
{
    m_ldInstance = nullptr;
    m_Worker = nullptr;
    m_workerThread = new QThread(this);

    connect(this, SIGNAL(missionCompleted(LDThread*)),APP_CTRL, SLOT(aMissionCompleted(LDThread*)));
    this->setLDInstance();
}

LDThread::~LDThread()
{
    LOG << m_ldInstance->instanceName();
    emit quitThread();
    m_workerThread->quit();
    m_workerThread->wait();
    m_ldInstance = nullptr;
    m_workerThread = nullptr;
}

void LDThread::setLDInstance()
{
    QMutex mutex;
    mutex.lock();
    if(APP_MODEL->latestRunningInstance() < APP_MODEL->devicesList().length()){
        APP_MODEL->setLatestRunningInstance(APP_MODEL->latestRunningInstance() + 1);

        m_ldInstance = dynamic_cast<LDIntance*>(APP_MODEL->devicesList().at(APP_MODEL->latestRunningInstance()));
        m_Worker = new LDRunner(m_ldInstance->instanceName());

        m_Worker->moveToThread(m_workerThread);
        connect(m_workerThread, &QThread::finished, m_Worker, &LDRunner::deleteLater);
        connect(this, &LDThread::operate, m_Worker, &LDRunner::run);
        connect(this, &LDThread::quitThread, m_Worker, &LDRunner::quitRunner);
        connect(m_Worker, &LDRunner::finished, this, &LDThread::finishedATask);
        m_workerThread->start();
        emit this->operate();
    }
    mutex.unlock();
}

void LDThread::finishedATask()
{
    emit missionCompleted(this);
}
