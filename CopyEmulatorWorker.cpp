#include "CopyEmulatorWorker.h"
#include "AppModel.h"
#include "LDIntance.h"

CopyEmulatorWorker::CopyEmulatorWorker(QObject *parent) : QObject(parent)
{

}

CopyEmulatorWorker::~CopyEmulatorWorker()
{
    LOG;
}

void CopyEmulatorWorker::doWork()
{
    LOG;
    QStringList existedDevices;
    existedDevices.clear();
    for (int i = 0; i < AppModel::instance()->devicesList().length(); i++) {
        existedDevices << dynamic_cast<LDIntance*>(AppModel::instance()->devicesList().at(i))->instanceName();
    }

    for (int i = 0; i < AppModel::instance()->deviceCount(); i++) {
        QString deviceName = EMULATOR_NAME_PREFIX + QString("-%1").arg(i);
        if(!existedDevices.contains(deviceName)){
            AppModel::instance()->setTaskInProgress(QString("Creating device: %1").arg(deviceName));
            LDCommand::coppyInstance(deviceName,ORIGIN_DEVICE_NAME);
            emit finishCopyDevice(deviceName);
            AppModel::instance()->setTaskInProgress("");
        }
    }
}
