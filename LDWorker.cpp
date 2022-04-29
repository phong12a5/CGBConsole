#include "LDWorker.h"
#include <AppDefines.h>
#include <LDCommand.h>
#include <QCoreApplication>
#include <QMutex>

#include "ScreenReferences.h"
#include "models/autofarmerapi.h"

using namespace screen_references;

LDWorker::LDWorker(int ldIndex, QObject *parent) :
    QObject(parent),
    m_ldIndex(ldIndex),
    m_started(false),
    openLD(false)
{
    connect(this, &LDWorker::post, this, &LDWorker::run, Qt::QueuedConnection);
}

LDWorker::~LDWorker()
{
    LOGD(m_ldIndex);
}

void LDWorker::start()
{
    LOGD(m_ldIndex);
    QMutex mutex;
    mutex.lock();
    m_started = true;
    postDelay(0);
    mutex.unlock();
}

void LDWorker::stop()
{
    LOGD(m_ldIndex);
    QMutex mutex;
    mutex.lock();
    m_started = false;
    mutex.unlock();
}

void LDWorker::run()
{
    if(!m_started) return;

    if(LDCommand::isRunningDevice(m_ldIndex) != LDCommand::DEVICE_STATE_RUNNING) {
        LDCommand::lunchInstance(m_ldIndex);
        QThread::msleep(5000);
    } else {
        if(player == nullptr){
            player = new LDPlayer(m_ldIndex);
        }

        player->openPackage("com.facebook.katana",true);

        QJsonObject acc = AutoFarmerAPI::instance()->getClone();
        LOGD(acc);
        if(!acc.isEmpty()){
            FacebookAccount *curAcc = FacebookAccount::fromJson(acc);
            int retry = 0;
            while (retry++<5) {
                if(player->getScreenId().contains(SCREEN_LOG_IN)){
                    break;
                }
                QThread::sleep(2);
            }
            if(retry<6){
                player->clickOn("Tên người dùng",CONTENT_DESCRIPTION);
                player->inputText(curAcc->getUsername());
                QThread::msleep(500);
                player->clickOn("Mật khẩu",CONTENT_DESCRIPTION);
                player->inputText(curAcc->getPassword());
                QThread::msleep(500);
                player->closeApp("com.facebook.katana");
                curAcc->deleteLater();
                QThread::sleep(2);
                QString path = ConfigHelper::getPackagePath().mid(0,ConfigHelper::getPackagePath().size()-11)+"backupPath";
                QDir directory(path);
                QStringList paths = directory.entryList(QStringList() << "*.tar",QDir::Files);
                int rd = Utility::random()%paths.size();
                while(!player->restoreFacebook(path+"/"+paths[rd])){
                    QThread::sleep(1);
                    LOGD("Restoring");
                }
                player->openPackage("com.facebook.katana");
                while (player->getScreenId().contains(SCREEN_UNKNOWN)) {
                    QThread::sleep(1);
                    LOGD("Waiting");
                }
            }else{

            }
        }
    }

    postDelay(2000);
}

void LDWorker::postDelay(int milSec)
{
    QThread::msleep(milSec);
    emit post();
}
