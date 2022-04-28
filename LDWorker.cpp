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
        LDCommand::ld_adb_command(m_ldIndex,"shell ime set com.android.adbkeyboard/.AdbIME");
        if(!LDCommand::isAppInForeground(m_ldIndex,"com.facebook.katana")) {
           LDCommand::enableFElement(m_ldIndex);
           LDCommand::runApp(m_ldIndex, "com.facebook.katana");
            QThread::msleep(7000);
        } else {
            if(player == nullptr){
                player = new LDPlayer(m_ldIndex);
            }

            QJsonObject acc = AutoFarmerAPI::instance()->getClone();
            if(!acc.isEmpty()){
                FacebookAccount *curAcc = FacebookAccount::fromJson(acc);
                int retry = 0;
                while (retry++<5) {
                    if(player->getScreenId().contains(SCREEN_LOG_IN)){
                        break;
                    }
                    QThread::sleep(1);
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
                }else{

                }
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
