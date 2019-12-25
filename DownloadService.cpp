#include "DownloadService.h"

DownloadService::DownloadService(QObject *parent) : QObject(parent)
{
    m_thread = new QThread();
    this->moveToThread(m_thread);
    connect(this, &DownloadService::destroyed, m_thread, &QThread::quit);
    connect(this, &DownloadService::operate, this, &DownloadService::run);
}

void DownloadService::run()
{
    LOGD("");
}
