#include "EmulatorWorker.h"
#include "AppModel.h"
#include "LDIntance.h"
#include <QDir>
#include <WebAPI.hpp>

#define APP_MODEL AppModel::instance()

EmulatorWorker::EmulatorWorker(QObject *parent) : QObject(parent)
{

}

EmulatorWorker::~EmulatorWorker()
{
    LOGD;
}

void EmulatorWorker::onCoppyDevices()
{
    LOGD;
    QStringList existedDevices;
    existedDevices.clear();
    for (int i = 0; i < AppModel::instance()->devicesList().length(); i++) {
        existedDevices << dynamic_cast<LDIntance*>(AppModel::instance()->devicesList().at(i))->instanceName();
    }

    for (int i = 0; i < AppModel::instance()->deviceCount(); i++) {
        QString deviceName = EMULATOR_NAME_PREFIX + QString("-%1").arg(i);
        if(!existedDevices.contains(deviceName)){
            AppModel::instance()->setTaskInProgress(QString("Creating device: %1").arg(deviceName));
            LDCommand::instance()->coppyInstance(deviceName,ORIGIN_DEVICE_NAME);
            emit finishCopyDevice(deviceName);
            AppModel::instance()->setTaskInProgress("");
        }
    }
}

void EmulatorWorker::onCreateTemplateDevice()
{
    /* --------- Check and download APK --------- */
    LOGD << "Downloading APK ...";
    APP_MODEL->setTaskInProgress("Downloading APK ...");
    delay(100);

    QString expectedApkFileName = QString(APK_FILENAME).arg(APP_MODEL->appConfig().m_android_versioncode);
    LOGD << "expectedApkFileName: " << expectedApkFileName;
    QDir directory(".");
    QStringList listApks = directory.entryList(QStringList() << "*.apk",QDir::Files);

    if(listApks.contains(expectedApkFileName)) {
        LOGD << expectedApkFileName << " is existed already";
    }else {
        if (!WebAPI::instance()->downloadApk(APP_MODEL->appConfig().m_android_versioncode)) {
            LOGD << "Download " << expectedApkFileName << " failure";
            if(!listApks.isEmpty())
                expectedApkFileName = listApks.last();
            else
                LOGD << "Couldn't get any apk file to install!";
        }else {
            LOGD << "Download " << expectedApkFileName << " successfully";
        }
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
    LDCommand::instance()->pushFile(deviceName,"/data/data/com.android.settings/databases","./databases");
    LDCommand::instance()->ld_adb_command(deviceName,"shell chown system:system /data/data/com.android.settings/databases/");
    LDCommand::instance()->ld_adb_command(deviceName,"shell chown system:system /data/data/com.android.settings/databases/su*");

    LDCommand::instance()->runLDCommand(QString("modify --name %1 --cpu 1 --memory 1024 --resolution %2").arg(ORIGIN_DEVICE_NAME).arg(APP_MODEL->resolution()));
    LDCommand::instance()->quitInstance(deviceName);
    APP_MODEL->setTaskInProgress("");
    APP_MODEL->setInitializing(false);
    emit finishCreateTemplateDevice();
}
