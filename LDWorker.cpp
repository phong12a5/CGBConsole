#include "LDWorker.h"
#include <AppDefines.h>
#include <LDCommand.h>
#include <QCoreApplication>
#include <QMutex>

#include "ScreenReferences.h"

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

//        LDCommand::runApp(m_name, "com.facebook.katana");
//        QThread::msleep(7000);
//        LDCommand::getElements(m_name);
//        LDCommand::killApp(m_name, "com.facebook.katana");


        if(!LDCommand::isAppInForeground(m_ldIndex,"com.facebook.katana")) {
           LDCommand::enableFElement(m_ldIndex);
           LDCommand::runApp(m_ldIndex, "com.facebook.katana");
            QThread::msleep(7000);
        } else {
            LDCommand::ld_adb_command(m_ldIndex, "shell input tap 38 166");
            QThread::msleep(2000);
            LDCommand::ld_adb_command(m_ldIndex, "shell input text 'Test_user'");
            QThread::msleep(2000);
            LDCommand::ld_adb_command(m_ldIndex, "shell input tap 38 199");
            QThread::msleep(2000);
            LDCommand::ld_adb_command(m_ldIndex, "shell input text 'ahihi'");
            QThread::msleep(2000);
            LDCommand::killApp(m_ldIndex, "com.facebook.katana");
        }


//        if(player == nullptr){
//            player = new LDPlayer(m_name);
////            while (!player->isAdbDeviceAvailable()) {
////                QThread::sleep(1);
////            }
//        }

//        if(!isGetElementOk){
//            LOGD("Open cgb");
//            //        player->openPackage("com.cgb.support");
//            LDCommand::runApp(m_name,"com.cgb.support");
//            while (!LDCommand::isAppInForeground(m_name,"com.cgb.support")) {
//                QThread::sleep(1);
//                LOGD(("Check cgb running"));
//            }
//            LDCommand::enableFElement(m_name);
//            if(player->getScreenId().size() == 0){
//                //            player->inputKeyEvent("KEYCODE_HOME");
//                return;
//            }else{
//                isGetElementOk = true;
//            }
//        }

//        if(!app_running) {
//            //        player->openPackage("com.facebook.katana",true);
//            LDCommand::runApp(m_name,"com.facebook.katana");
//            app_running = true;
//        } else {
//            if(isGetElementOk){
//                while (true) {
//                    if(player->getScreenId().contains(SCREEN_LOG_IN)){
//                        break;
//                    }
////                    QCoreApplication::processEvents();
//                    QThread::sleep(1);
//                }

//                player->clickOn("Tên người dùng",CONTENT_DESCRIPTION);
//                player->inputText("Test_user");
////                QCoreApplication::processEvents();
//                QThread::sleep(1);
//                player->clickOn("Mật khẩu",CONTENT_DESCRIPTION);
//                player->inputText("12345");
////                QCoreApplication::processEvents();
//                QThread::sleep(1);
//            }
//            LDCommand::killApp(m_name, "com.facebook.katana");
//            app_running = false;
//        }
    }

    postDelay(2000);
}

void LDWorker::postDelay(int milSec)
{
    QThread::msleep(milSec);
    emit post();
}
