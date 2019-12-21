#include "AutoUpdaterWorker.h"
#include "AppDefines.h"
#include "WebAPI.hpp"
#include <QFile>
#include <CkZip.h>
#include "AppModel.h"

AutoUpdaterWorker::AutoUpdaterWorker(QObject *parent) : QObject(parent)
{
    m_checkingTimer = nullptr;
}

void AutoUpdaterWorker::onStartAutoUpdater()
{
    LOGD;
    if(m_checkingTimer == nullptr){
        m_checkingTimer = new QTimer(this);
        m_checkingTimer->setInterval(15  * 60 * 1000);
        m_checkingTimer->setSingleShot(false);
        connect(m_checkingTimer, &QTimer::timeout, this, &AutoUpdaterWorker::onCheckUpdate);
        m_checkingTimer->start();
        this->onCheckUpdate();
    }
}

void AutoUpdaterWorker::onCheckUpdate()
{
    if(WebAPI::instance()->getConfig()){
        LOGD << "Current version:" << AppModel::instance()->versionCode();
        LOGD << "New version:" << AppModel::instance()->appConfig().m_cgbconsole_versioncode;
        if(AppModel::instance()->versionCode() != AppModel::instance()->appConfig().m_cgbconsole_versioncode){
            if(WebAPI::instance()->downloadNewVersion()){
                CkZip zip;

                if(QFile::rename("CGBConsole.exe","temp.dat") == true){
                    if (zip.OpenZip("CGBConsole.zip") != true) {
                        LOGD << "zip.lastErrorText(): " << zip.lastErrorText();
                        QFile::rename("temp.dat","CGBConsole.exe");
                    }else {
                        int unzipCount;
                        unzipCount = zip.Unzip(".");
                        if (unzipCount < 0) {
                            LOGD << "zip.lastErrorText(): "  << zip.lastErrorText();
                            QFile::rename("temp.dat","CGBConsole.exe");
                        } else {
                            LOGD << "Update new version successful";
                            emit updateFinished(E_FINISHED_CODE_NEW_VERSION);
                            return;
                        }
                        zip.CloseZip();
                    }
                }
                QFile::remove("CGBConsole.zip");
            }
            LOGD << "Update new version failure";
            emit updateFinished(E_FINISHED_CODE_UPDATE_FAILURE);
        }else {
            LOGD << "Current version is latest!";
        }
    }else {
        LOGD << "Failed to getConfig";
    }
}
