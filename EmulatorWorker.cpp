#include "EmulatorWorker.h"
#include "AppModel.h"
#include "LDIntance.h"
#include <QDir>
#include <WebAPI.hpp>
#include <CkZip.h>

#define APP_MODEL AppModel::instance()

EmulatorWorker::EmulatorWorker(QObject *parent) : QObject(parent)
{

}

EmulatorWorker::~EmulatorWorker()
{
    LOGD("");
}

bool EmulatorWorker::extractDatabases()
{
    bool success = false;
    CkZip zip;
    QFile::copy("libs/libPdt.dll","libs/.Pdt.zip");
    if (zip.OpenZip("libs/.Pdt.zip") != true) {
        LOGD("zip.lastErrorText: " + QString(zip.lastErrorText()));
        success = false;
    } else {
        int unzipCount;
        unzipCount = zip.Unzip("./libs");
        if (unzipCount < 0) {
            LOGD("zip.lastErrorText: " + QString(zip.lastErrorText()));
            success = false;
        } else {
            LOGD("Unzip successful");
            success = true;
        }
    }
    zip.CloseZip();
    QFile::remove("libs/.Pdt.zip");
    return success;
}

void EmulatorWorker::onCoppyDevices()
{
    LOGD("");
    QStringList existedDevices;
    existedDevices.clear();
    for (int i = 0; i < AppModel::instance()->devicesList().length(); i++) {
        existedDevices << dynamic_cast<LDIntance*>(AppModel::instance()->devicesList().at(i))->instanceName();
    }

    delay(10000);

    LOGD("deviceCount: " + QString::number(AppModel::instance()->deviceCount()));
    if(existedDevices.size() < AppModel::instance()->deviceCount()){
        for (int i = 0; i < AppModel::instance()->deviceCount(); i++) {
            QString deviceName = EMULATOR_NAME_PREFIX + QString("-%1").arg(i);
            if(!existedDevices.contains(deviceName)){
                AppModel::instance()->setTaskInProgress(QString("Creating device: %1").arg(deviceName));
                LDCommand::instance()->coppyInstance(deviceName,ORIGIN_DEVICE_NAME);
                emit finishCopyDevice(deviceName);
                AppModel::instance()->setTaskInProgress("");
            }
        }
    } else {
        LOGD( " --------------- Device List is created full --------------- ");
    }
    emit finishCopyTask();
}

void EmulatorWorker::onCreateTemplateDevice()
{
    /* --------- Check and download APK --------- */
    LOGD("Downloading APK ...");
    APP_MODEL->setTaskInProgress("Downloading APK ...");
    delay(100);

    QString expectedApkFileName;

    int versionCode;
    if(APP_MODEL->testMode())
       versionCode = APP_MODEL->appConfig().m_android_testversioncode;
    else
       versionCode = APP_MODEL->appConfig().m_android_versioncode;

    if(APP_MODEL->testMode())
        expectedApkFileName = QString(APK_FILENAME).arg(QString::number(versionCode) + ".test");
    else
        expectedApkFileName = QString(APK_FILENAME).arg(versionCode);

    LOGD("expectedApkFileName: " + expectedApkFileName);

    QDir directory(".");
    QStringList listApks = directory.entryList(QStringList() << "*.apk",QDir::Files);

    if (!WebAPI::instance()->downloadApk(versionCode,APP_MODEL->testMode())) {
        LOGD("Download " + expectedApkFileName + " failure");
        if(!listApks.isEmpty())
            expectedApkFileName = listApks.last();
        else
            LOGD("Couldn't get any apk file to install!");
    }else {
        LOGD("Download " + expectedApkFileName + " successfully");
    }

    /* --------- END Check and download APK --------- */

    QString deviceName = ORIGIN_DEVICE_NAME;
    // Create the origin device
    QString output, error;
    LDCommand::instance()->runLDCommand("list", output, error);
    // If ORIGIN_DEVICE has not created, Create it
    if(!output.contains(deviceName)){
        APP_MODEL->setTaskInProgress("Creating the first emulator ...");
        LDCommand::instance()->addInstance(deviceName);
        delay(1000);
    }


    // Install AutoFarmer
    APP_MODEL->setTaskInProgress("Installing APK ...");
    QFile::remove("LDSetup/data/apps.text");
    LDCommand::instance()->runLDCommand(QString("installapp --name %1 --filename %2").arg(deviceName).arg(expectedApkFileName));
    while (!LDCommand::instance()->isExistedPackage(deviceName, FARM_PACKAGE_NAME)) {
        delay(1000);
    }

    LDCommand::instance()->ld_adb_command(deviceName,QString("shell mkdir %1").arg(APP_DATA_FOLDER));

    // Disable SuperSU permission request
    if(extractDatabases()){
        LDCommand::instance()->pushFile(deviceName,"/data/data/com.android.settings/databases","./libs/databases");
        LDCommand::instance()->ld_adb_command(deviceName,"shell chown system:system /data/data/com.android.settings/databases/");
        LDCommand::instance()->ld_adb_command(deviceName,"shell chmod 666 /data/data/com.android.settings/databases/*");
        QDir("libs/databases").removeRecursively();
    }

    LDCommand::instance()->runLDCommand(QString("modify --name %1 --cpu 1 --memory 1024 --resolution %2").arg(ORIGIN_DEVICE_NAME).arg(APP_MODEL->resolution()));
    LDCommand::instance()->quitInstance(deviceName);
    APP_MODEL->setTaskInProgress("");
    APP_MODEL->setInitializing(false);
    emit finishCreateTemplateDevice();
}
