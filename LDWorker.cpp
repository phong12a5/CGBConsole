#include "LDWorker.h"
#include <AppDefines.h>
#include <LDCommand.h>
#include <QCoreApplication>
#include <QMutex>

#include "ScreenReferences.h"

using namespace screen_references;

LDWorker::LDWorker(QString name, QObject *parent) :
    QObject(parent),
    m_name(name),
    m_started(false)
{
    connect(this, &LDWorker::post, this, &LDWorker::run, Qt::QueuedConnection);
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
    LDCommand::lunchInstance(m_name);
    postDelay(0);
    mutex.unlock();
}

void LDWorker::stop()
{
    LOGD(m_name);
    QMutex mutex;
    mutex.lock();
    m_started = false;
    mutex.unlock();
}

void LDWorker::run()
{
    if(!m_started) return;

    if(player == nullptr){
        player = new LDPlayer(m_name);
        while (!player->isAdbDeviceAvailable()) {
            QThread::sleep(1);
        }
    }

    if(!isGetElementOk){
        LOGD("Open cgb");
        player->openPackage("com.cgb.support");
        while (!player->isAppRunning("com.cgb.support")) {
            QThread::sleep(1);
            LOGD(("Check cgb running"));
        }
        player->enableAccessibility("com.cgb.support",".service.QAccessibilityService");
        if(player->getScreenId().size() == 0){
            player->inputKeyEvent("KEYCODE_HOME");
            return;
        }else{
            isGetElementOk = true;
        }
    }

    if(!app_running) {
        player->openPackage("com.facebook.katana",true);

        app_running = true;
    } else {
        if(isGetElementOk){
            while (true) {
                if(player->getScreenId().contains(SCREEN_LOG_IN)){
                    break;
                }
                QCoreApplication::processEvents();QThread::sleep(1);
            }

            player->clickOn("Tên người dùng",CONTENT_DESCRIPTION);
            player->inputText("Test_user");
            QCoreApplication::processEvents();QThread::sleep(1);
            player->clickOn("Mật khẩu",CONTENT_DESCRIPTION);
            player->inputText("12345");
            QCoreApplication::processEvents();QThread::sleep(1);
            player->closeApp("com.facebook.katana");
        }
        LDCommand::killApp(m_name, "com.facebook.katana");
        app_running = false;
    }

    postDelay(1000);
}

void LDWorker::postDelay(int milSec)
{
    QThread::msleep(milSec);
    emit post();
}
