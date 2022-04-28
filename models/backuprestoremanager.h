#ifndef BACKUPRESTOREMANAGER_H
#define BACKUPRESTOREMANAGER_H

#include <QObject>
#include "backuprestorepackage.h"

class BackupRestoreManager : public QObject
{
    Q_OBJECT
public:
    static BackupRestoreManager *instance();

    BackupRestorePackage* nextPackage();
    void updatePackage(BackupRestorePackage &package);

    void addPackage(BackupRestorePackage *package);
    void removePackage(BackupRestorePackage *package);


    QList<QString> getSavedAccount();
    int getValidPackageCount();
    void reloadPackage();
    int getUpdatedCount();
private:
    QMutex mutex;

    static BackupRestoreManager* m_instance;
    explicit BackupRestoreManager(QObject *parent = nullptr);
    ~BackupRestoreManager();
    QList<BackupRestorePackage*> m_listPackage;
    int m_currentId = -1;

    void loadPackage();
    void generateData();
    int m_packageReloadCount = 0;
    int m_updatedCount=0;
};

#endif // BACKUPRESTOREMANAGER_H
