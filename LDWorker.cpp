#include "LDWorker.h"
#include <AppDefines.h>
#include <LDCommand.h>
#include <QCoreApplication>
#include <QMutex>
#include <QDebug>

#include "ScreenReferences.h"
#include "models/autofarmerapi.h"
#include "qgoogleauthenticator.hpp"

#define LOG qDebug()<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")<<"ID:"<<(player==nullptr?-1:player->getProfile().id)<<__FUNCTION__<<__LINE__
#define sleep QThread::sleep
#define msleep QThread::msleep

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
            player->setupProxy();
        }

//        State rs = preparePackage();
//        if(rs == PREPARE_SUCCESS){
//            player->swipeUp();
//            sleep(5);
//            LOG<<"prepare done";
//        }
    }

    postDelay(2000);
}

void LDWorker::postDelay(int milSec)
{
    QThread::msleep(milSec);
    emit post();
}

//private
LDWorker::State LDWorker::doLogin(FacebookAccount &acc)
{
    LOG<<"acc:"<<acc.getUsername()<<acc.getPassword();
//    MES,"Đang mở Facebook...");

    LOG<<":Install FB";
    while (!player->isInstallApp("com.facebook.katana")) {
        if(!player->installApk(ConfigHelper::getFacebookApkPath())){
            LOG<<"Install failed";
        }
        msleep(500);
    }
    LOG<<":FB is installed";
    LOG<<":OpenFacebook";
    player->openPackage("com.facebook.katana",true);
    int unknownCount = 0;
    int loadVietnameseCount = 0;
    bool isInEngLish = false;
    int errorCodeCount = 0;
    for(int i=0;i<40;i++){
        QJsonArray currentScreenId = player->getScreenId();
        LOG<<"i: "<<i<<" "<<currentScreenId;
        if(currentScreenId.contains(SCREEN_UNKNOWN)){
            unknownCount++;
            if(unknownCount >= 5){
//                player->openPackage("com.facebook.katana",false);
                unknownCount = 0;
            }
        }else if(currentScreenId.contains(SCREEN_LOG_IN)){
            player->clickOn("Tên người dùng",CONTENT_DESCRIPTION);
            player->inputText(acc.getUsername());
            sleep(1);
            player->clickOn("Mật khẩu",CONTENT_DESCRIPTION);
            player->inputText(acc.getPassword());
            sleep(1);
            player->clickOn("Đăng nhập",CONTENT_DESCRIPTION);
            sleep(3);
        }else if(currentScreenId.contains(SCREEN_CODE_2FA_DIALOG)){
            player->clickOn("OK",TEXT);
        }else if(currentScreenId.contains(SCREEN_INPUT_2FA_CODE)){
            player->clickOn("Mã đăng nhập",TEXT);
            player->inputText(QGoogleAuthenticator::getCode(acc.get2fa().toUtf8()));
            player->clickOn("Tiếp tục",CONTENT_DESCRIPTION);
            sleep(5);
        }else if(currentScreenId.contains(SCREEN_SAVE_INFO)){
            player->clickOn("OK",TEXT);
        }else if(currentScreenId.contains(SCREEN_FIND_FRIEND)){
            player->clickOn("Bỏ qua",CONTENT_DESCRIPTION);
        }else if(currentScreenId.contains(SCREEN_REQUEST_ACCESS_LOCATION)){
            player->clickOn("Từ chối",CONTENT_DESCRIPTION);
        }else if(currentScreenId.contains(SCREEN_HOME)){
            player->swipeUp();
            if(isInEngLish){
                return LOAD_VIETNAMESE_FAILED;
            }else{
                return CONTINUE;
            }
        }else if(currentScreenId.contains(SCREEN_CODE_EXPIRED)){
            player->clickOn("OK",TEXT);
            if(errorCodeCount++>=3){
//                return NEW_LOOP;
                break;
            }
        }else if(currentScreenId.contains(SCREEN_CHECK_POINT)){
            m_package->setAccount(FacebookAccount::fromJson(QJsonObject()));
            return GET_CLONE;
        }else if(currentScreenId.contains(SCREEN_LOST_INTERNET_CONNECTION)){
//            return NEW_LOOP;
            break;
        }else if(currentScreenId.contains(SCREEN_CONFIRM_INORGE_FIND_FRIEND)){
            player->clickOn("BỎ QUA",TEXT);
        }else if(currentScreenId.contains(SCREEN_LOAD_VIETNAMESE_FAILED)){
            player->clickOn("TIẾP TỤC BẰNG TIẾNG ANH (MỸ)",TEXT);
            isInEngLish = true;
        }else if(currentScreenId.contains(SCREEN_REVIEW_DATA_DONE)){
            player->clickOn("ĐÓNG",CONTENT_DESCRIPTION);
        }else if(currentScreenId.contains(SCREEN_REVIEW_DATA_5)){
            player->clickOn("TIẾP TỤC",CONTENT_DESCRIPTION);
        }else if(currentScreenId.contains(SCREEN_REVIEW_DATA_4)){

        }else if(currentScreenId.contains(SCREEN_REVIEW_DATA_3)
                 || currentScreenId.contains(SCREEN_REVIEW_DATA_2)){
            player->clickOn("CHẤP NHẬN VÀ TIẾP TỤC",CONTENT_DESCRIPTION);
        }else if(currentScreenId.contains(SCREEN_REVIEW_DATA_1)){
            player->clickOn("BẮT ĐẦU",CONTENT_DESCRIPTION);
        }else if(currentScreenId.contains(SCREEN_ADD_AVATAR)){
            player->clickOn("Bỏ qua",TEXT);
        }
        sleep(1);
    }
//    MES,"Login failed");
    return FINISH;
}

LDWorker::State LDWorker::preparePackage()
{
    State rs = UNKNOWN_STATE;
    if(AutoFarmerAPI::instance()->hasClone()){
        //has new clone
        if(player->fakeDevice()){
            //fake device success
            QJsonObject acc = AutoFarmerAPI::instance()->getClone();
            if(!acc.isEmpty()){
                //get device success
                FacebookAccount *curAcc = FacebookAccount::fromJson(acc);
                rs = doLogin(*curAcc);
                if(rs == CONTINUE){
                    m_package = BackupRestorePackage::fromJson(QJsonObject());
                    m_package->setAccount(curAcc);

                    QString backupPath = ConfigHelper::getPackagePath().remove("package.ifo")+"backupPath";
                    if(!QDir(backupPath).exists()){
                        QDir().mkdir(backupPath);
                    }
//                    MES,"Backup facebook");
                    int retryToBackup = 0;
                    QString backupFile = "";
                    while(backupFile == ""
                            || !QFile::exists(backupFile)){
                        backupFile = player->backupFacebook(m_package->account()->getUsername(),backupPath);
                        if(retryToBackup++>3){
                            break;
                        }
                    }

                    if(retryToBackup<5){
                        m_package->setBackupPath(backupFile);
                        m_package->setDeviceInfo(DeviceInfo::fromJson(QJsonObject()));

                        QJsonDocument doc = QJsonDocument::fromJson(player->getXposedData().toUtf8());
                        if(!doc.isNull()
                                && !doc.isEmpty()
                                && doc.object().contains("buildProp")
                                && doc.object().contains("other")){
                            m_package->deviceInfo()->setXposedData(doc.object());
                            BackupRestoreManager::instance()->addPackage(BackupRestorePackage::fromJson(m_package->toJson()));
                            return PREPARE_SUCCESS;
                        }
                    }
                }
            }
        }
    }else{
        m_package = BackupRestoreManager::instance()->nextPackage();
        //restore
        LOG<<"Restore device";
//        MES,"Retore device Info : "+m_package->deviceInfo()->modelName());
        player->restoreDeviceInfo(m_package->deviceInfo()->xposeData());
//                sleep(2);
        player->openPackage("com.facebook.katana");
        sleep(5);
        for(int i=0;i<30;i++){
            QJsonArray currentScreenId = player->getScreenId();
            LOG<<currentScreenId;
            if(currentScreenId.contains(SCREEN_HOME)){
                return PREPARE_SUCCESS;
            }else if(currentScreenId.contains(SCREEN_LOAD_VIETNAMESE_FAILED)){
                player->clickOn("TIẾP TỤC BẰNG TIẾNG ANH (MỸ)",TEXT);
            }else if(currentScreenId.contains(SCREEN_LOG_IN)){
                QFile(m_package->backupPath()).remove();
                m_package->setBackupPath("");
                m_package->setDeviceInfo(DeviceInfo::fromJson(QJsonObject()));
                break;
            }else if(currentScreenId.contains(SCREEN_SESSION_EXPIRED)){
                m_package->setBackupPath("");
                m_package->setDeviceInfo(DeviceInfo::fromJson(QJsonObject()));
                break;
            }else if(currentScreenId.contains(SCREEN_FACEBOOK_NOT_RESPONSE)){
                break;
            }else if(currentScreenId.contains(SCREEN_CHECK_POINT)
                     || currentScreenId.contains(SCREEN_LOST_INTERNET_CONNECTION)){
                m_package->setAccount(FacebookAccount::fromJson(QJsonObject()));
                m_package->setBackupPath("");
                m_package->setDeviceInfo(DeviceInfo::fromJson(QJsonObject()));
                break;
            }else if(currentScreenId.contains(SCREEN_SAVE_INFO)){
                player->clickOn("OK",TEXT);
            }

            sleep(1);
            if(i==29){
                player->setupProxy();
            }
        }
    }
    return PREPARE_FAILED;
}
