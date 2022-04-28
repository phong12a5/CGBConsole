#ifndef BACKUPRESTOREPACKAGE_H
#define BACKUPRESTOREPACKAGE_H

#include <QObject>
#include "facebookaccount.h"
#include "deviceinfo.h"

class BackupRestorePackage : public QObject
{
    Q_OBJECT
public:
    explicit BackupRestorePackage(QObject *parent = nullptr,
                                  int packageIndex = -1,
                                  QJsonObject account = QJsonObject(),
                                  QJsonObject deviceInfo = QJsonObject(),
                                  QString backupPath = "");
    ~BackupRestorePackage();

    int packageIndex() const;
    void setPackageIndex(int packageIndex);

    FacebookAccount *account() const;
    void setAccount(FacebookAccount *account);

    QString backupPath() const;
    void setBackupPath(const QString &backupPath);

    static BackupRestorePackage* fromJson(QJsonObject obj);
    QJsonObject toJson();

    DeviceInfo *deviceInfo() const;
    void setDeviceInfo(DeviceInfo *deviceInfo);

    bool isValidPackage();

    bool isInUsing() const;
    void setIsInUsing(bool isInUsing);

signals:
private:
    int m_packageIndex = -1;
    FacebookAccount* m_account{nullptr};
    DeviceInfo *m_deviceInfo;
    QString m_backupPath = "";
    bool m_isInUsing = false;

};

#endif // BACKUPRESTOREPACKAGE_H
