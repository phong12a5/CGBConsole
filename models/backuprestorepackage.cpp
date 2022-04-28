#include "backuprestorepackage.h"

BackupRestorePackage::BackupRestorePackage(QObject *parent, int packageIndex, QJsonObject account, QJsonObject deviceInfo, QString backupPath):
    QObject(parent),
    m_packageIndex(packageIndex),
    m_account(FacebookAccount::fromJson(account)),
    m_deviceInfo(DeviceInfo::fromJson(deviceInfo)),
    m_backupPath(backupPath)
{

}

BackupRestorePackage::~BackupRestorePackage()
{
    if(m_account != nullptr){
        m_account->deleteLater();
        m_account = nullptr;
    }

    if(m_deviceInfo != nullptr){
        m_deviceInfo->deleteLater();
        m_deviceInfo = nullptr;
    }
}

int BackupRestorePackage::packageIndex() const
{
    return m_packageIndex;
}

void BackupRestorePackage::setPackageIndex(int packageIndex)
{
    m_packageIndex = packageIndex;
}

FacebookAccount *BackupRestorePackage::account() const
{
    return m_account;
}

void BackupRestorePackage::setAccount(FacebookAccount *account)
{
    if(m_account != nullptr){
        delete m_account;
        m_account = nullptr;
    }
    m_account = account;
}

QString BackupRestorePackage::backupPath() const
{
    return m_backupPath;
}

void BackupRestorePackage::setBackupPath(const QString &backupPath)
{
    m_backupPath = backupPath;
}

BackupRestorePackage* BackupRestorePackage::fromJson(QJsonObject obj)
{
    BackupRestorePackage *package = new BackupRestorePackage();
    if(obj.contains("packageIndex")){
        package->setPackageIndex(obj["packageIndex"].toInt());
    }
    if(obj.contains("account")){
        package->setAccount(FacebookAccount::fromJson(obj["account"].toObject()));
    }
    if(obj.contains("deviceInfo")){
        package->setDeviceInfo(DeviceInfo::fromJson(obj["deviceInfo"].toObject()));
    }
    if(obj.contains("backupPath")){
        package->setBackupPath(obj["backupPath"].toString());
    }

    return package;
}

QJsonObject BackupRestorePackage::toJson()
{
    QJsonObject obj;
    obj.insert("packageIndex",m_packageIndex);
    if(m_account->getUsername() == ""){
        obj.insert("account",QJsonObject());
    }else{
        obj.insert("account",m_account->toJsonObject());
    }
    obj.insert("deviceInfo",m_deviceInfo->toJson());
    obj.insert("backupPath",m_backupPath);

    return obj;
}

DeviceInfo *BackupRestorePackage::deviceInfo() const
{
    return m_deviceInfo;
}

void BackupRestorePackage::setDeviceInfo(DeviceInfo *deviceInfo)
{
    if(m_deviceInfo != nullptr){
        delete m_deviceInfo;
        m_deviceInfo = nullptr;
    }
    m_deviceInfo = deviceInfo;
}

bool BackupRestorePackage::isValidPackage()
{
    if(m_account->getUsername() != ""
            && m_backupPath != ""
            && !m_deviceInfo->xposeData().isEmpty()
            && m_backupPath.contains(m_account->getUsername())){
        return true;
    }
    return false;
}

bool BackupRestorePackage::isInUsing() const
{
    return m_isInUsing;
}

void BackupRestorePackage::setIsInUsing(bool isInUsing)
{
    m_isInUsing = isInUsing;
}
