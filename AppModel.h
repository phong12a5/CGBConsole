#ifndef APPMODEL_H
#define APPMODEL_H

#include <QObject>
#include "AppDefines.h"
#include "LDCommand.h"
#include "LDIntance.h"
#include <QMutex>

class AppModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ldIntallFolder READ ldIntallFolder WRITE setLDIntallFolder NOTIFY ldIntallFolderChanged)
    Q_PROPERTY(QList<QObject*> devicesList READ devicesList WRITE setDevicesList NOTIFY devicesListChanged)
    Q_PROPERTY(uint amountOfThread READ amountOfThread WRITE setAmountOfThread NOTIFY amountOfThreadChanged)
    Q_PROPERTY(bool isLaunchMutiTask READ isLaunchMutiTask WRITE setIsLaunchMutiTask NOTIFY isLaunchMutiTaskChanged)
    Q_PROPERTY(QString token READ token WRITE setToken NOTIFY tokenChanged)
    Q_PROPERTY(int maxNumberThread READ maxNumberThread CONSTANT)
    Q_PROPERTY(bool initializing READ initializing WRITE setInitializing NOTIFY initializingChanged)
    Q_PROPERTY(QString appName READ appName WRITE setAppName NOTIFY appNameChanged)
    Q_PROPERTY(bool walletEmpty READ walletEmpty WRITE setWalletEmpty NOTIFY walletEmptyChanged)

private:
    explicit AppModel(QObject *parent = nullptr);

public:
    static AppModel* instance();

    QString ldIntallFolder() const;
    void setLDIntallFolder(const QString path, bool standardPath = false);

    QList<QObject*> devicesList() const;
    void setDevicesList(const QList<QObject*> devices);

    uint amountOfThread() const;
    void setAmountOfThread(const uint data);

    bool isLaunchMutiTask() const;
    void setIsLaunchMutiTask(const bool data);

    int latestRunningInstance() const;
    void setLatestRunningInstance(const int data);

    QString token() const;
    void setToken(QString data);

    APP_CONFIG appConfig() const;
    void setAppConfig(APP_CONFIG data);

    int maxNumberThread() const;

    bool initializing() const;
    void setInitializing(bool data);

    QString currentDir() const;
    void setCurrentDir(QString data);

    QString appName() const;
    void setAppName(QString data);

    bool walletEmpty() const;
    void setWalletEmpty(bool data);

public:
    Q_INVOKABLE void startProgram();
    Q_INVOKABLE void stopProgarm();

signals:
    void ldIntallFolderChanged();
    void devicesListChanged();
    void amountOfThreadChanged();
    void isLaunchMutiTaskChanged();
    void tokenChanged();
    void initializingChanged();
    void appNameChanged();
    void walletEmptyChanged();

    void reInitDeviceList();

    void sigStartProgram();
    void sigStoptProgram();

private:
    static AppModel* m_instance;

    QString m_ldIntallFolder;
    QList<QObject*> m_devicesList;
    uint m_amountOfThread;
    bool m_isLaunchMutiTask;
    int m_latestRunningInstance;
    QString m_token;
    APP_CONFIG m_appConfig;
    bool m_initializing;
    QString m_currentDir;
    QString m_appName;
    bool m_walletEmpty;

public slots:
};

#endif // APPMODEL_H
