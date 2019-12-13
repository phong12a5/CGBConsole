#include "AutoUpdaterWorker.h"
#include "AppDefines.h"
#include "WebAPI.hpp"

AutoUpdaterWorker::AutoUpdaterWorker(QObject *parent) : QObject(parent)
{

}

void AutoUpdaterWorker::doWork()
{
    LOGD;
    if(WebAPI::instance()->downloadNewVersion()){
        emit updateFinished(E_FINISHED_CODE_NEW_VERSION);
    }else {
        emit updateFinished(E_FINISHED_CODE_UPDATE_FAILURE);
    }
}
