#include "backuprestoremanager.h"

#include "ConfigHelper.h"

BackupRestoreManager* BackupRestoreManager::m_instance = nullptr;

BackupRestoreManager* BackupRestoreManager::instance()
{
    if(m_instance == nullptr){
        m_instance = new BackupRestoreManager();
    }
    return m_instance;
}

BackupRestoreManager::BackupRestoreManager(QObject *parent) : QObject(parent)
{
    m_listPackage = QList<BackupRestorePackage*>();
    loadPackage();
}

BackupRestoreManager::~BackupRestoreManager()
{

}

BackupRestorePackage* BackupRestoreManager::nextPackage()
{
    QMutexLocker locker(&mutex);
    m_currentId++;
//    m_currentId = 1;
    if(m_currentId>= m_listPackage.size()){
        m_currentId = 0;
    }
    qDebug()<<"Current Package :"<<m_currentId;
    if(QDateTime::currentDateTime().toSecsSinceEpoch()%6000==0){
        QFile::copy(ConfigHelper::getPackagePath(),ConfigHelper::getPackagePath().remove("package.ifo")+"/package"+Utility::generateCurrentTime()+".ifo");
    }

    for(int i=0;i<m_listPackage.size();i++){
        if(m_listPackage[i]->packageIndex() == m_currentId){
            m_listPackage[i]->setIsInUsing(true);
            return m_listPackage[i];
        }
    }

    return nullptr;
}

void BackupRestoreManager::updatePackage(BackupRestorePackage &package)
{
    int pIndex = package.packageIndex();

}

void BackupRestoreManager::addPackage(BackupRestorePackage *package)
{
    QMutexLocker locker(&mutex);
    m_listPackage.append(package);
    reloadPackage();
}

void BackupRestoreManager::removePackage(BackupRestorePackage *package)
{
    QMutexLocker locker(&mutex);
    m_listPackage.removeAll(package);
    package->deleteLater();
    reloadPackage();
}

QList<QString> BackupRestoreManager::getSavedAccount()
{
    QList<QString> loggedIn = QList<QString>();
    for(int i=0;i<m_listPackage.size();i++){
        loggedIn.append(m_listPackage[i]->account()->getUsername());
    }
    return loggedIn;
}

int BackupRestoreManager::getValidPackageCount()
{
    int validCount = 0;
    for(int i=0;i<m_listPackage.size();i++){
        if(m_listPackage[i]->account()->getUsername() != ""
                && m_listPackage[i]->backupPath() != ""
                && !m_listPackage[i]->deviceInfo()->xposeData().isEmpty()
                && m_listPackage[i]->backupPath().contains(m_listPackage[i]->account()->getUsername())){
            validCount++;
        }
    }
    return validCount;
}
//private
void BackupRestoreManager::loadPackage()
{
    QString packagePath = ConfigHelper::getPackagePath();
    QFileInfo info(packagePath);
    if(info.exists()){
        QString read = Utility::read(packagePath);
        if(read.size() < 2){
            QDir dir(packagePath.mid(0,packagePath.size()-12));
            QFileInfoList backups = dir.entryInfoList(QStringList() << "*.ifo",QDir::Files);
            int lastModifiIndex = -1;
            int secToCurrentTime = 0;
            QDateTime currentTime = QDateTime::currentDateTime();
            for(int i=0;i<backups.size();i++){
                if(backups[i].size() == 0){
                    continue;
                }
                if(secToCurrentTime == 0){
                    secToCurrentTime = backups[i].lastModified().secsTo(currentTime);
                    lastModifiIndex = i;
                }else{
                    if(secToCurrentTime > backups[i].lastModified().secsTo(currentTime) ){
                        secToCurrentTime = backups[i].lastModified().secsTo(currentTime);
                        lastModifiIndex = i;
                    }
                }
            }
            if(lastModifiIndex!=-1){
                read = Utility::read(backups[lastModifiIndex].filePath());
                if(read.size() < 2){
                    generateData();
                    return;
                }
            }else{
                generateData();
                return;
            }
        }
        QJsonDocument doc = QJsonDocument::fromJson(read.toUtf8());
        if(!doc.isNull()
                && !doc.isEmpty()){
            QJsonArray data = doc.array();
            m_listPackage.clear();
            for(int i=0;i<data.size();i++){
                m_listPackage.append(BackupRestorePackage::fromJson(data[i].toObject()));
            }
        }
    }else{
        generateData();
    }
}

void BackupRestoreManager::generateData()
{
    m_currentId = 0;
    QJsonArray package;
    Utility::write(ConfigHelper::getPackagePath(),QJsonDocument(package).toJson(QJsonDocument::Compact));
}

int BackupRestoreManager::getUpdatedCount()
{
    return m_updatedCount;
}

void BackupRestoreManager::reloadPackage()
{
    qDebug()<<__FUNCTION__;
    m_packageReloadCount++;
    m_updatedCount = 0;
    QJsonArray arr;
    for(int i=0;i<m_listPackage.size();i++){
        arr.append(m_listPackage[i]->toJson());
    }
    Utility::write(ConfigHelper::getPackagePath(),QJsonDocument(arr).toJson(QJsonDocument::Compact));
}
