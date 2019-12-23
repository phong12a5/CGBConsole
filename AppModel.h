#ifndef APPMODEL_H
#define APPMODEL_H

#include <QObject>
#include "AppDefines.h"
#include "LDCommand.h"
#include "LDIntance.h"
#include <QMutex>
#include <QSize>

class AppModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ldIntallFolder READ ldIntallFolder WRITE setLDIntallFolder NOTIFY ldIntallFolderChanged)
    Q_PROPERTY(QList<QObject*> devicesList READ devicesList NOTIFY devicesListChanged)
    Q_PROPERTY(QList<QObject*> devicesRunningList READ devicesRunningList NOTIFY devicesRunningListChanged)
    Q_PROPERTY(uint amountOfThread READ amountOfThread WRITE setAmountOfThread NOTIFY amountOfThreadChanged)
    Q_PROPERTY(bool isLaunchMutiTask READ isLaunchMutiTask WRITE setIsLaunchMutiTask NOTIFY isLaunchMutiTaskChanged)
    Q_PROPERTY(QString token READ token WRITE setToken NOTIFY tokenChanged)
    Q_PROPERTY(int maxNumberThread READ maxNumberThread WRITE setMaxNumberThread NOTIFY maxNumberThreadChanged)
    Q_PROPERTY(int maxVMCount READ maxVMCount WRITE setMaxVMCount NOTIFY maxVMCountChanged)
    Q_PROPERTY(bool initializing READ initializing WRITE setInitializing NOTIFY initializingChanged)
    Q_PROPERTY(QString appName READ appName WRITE setAppName NOTIFY appNameChanged)
    Q_PROPERTY(bool walletEmpty READ walletEmpty WRITE setWalletEmpty NOTIFY walletEmptyChanged)
    Q_PROPERTY(int deviceCount READ deviceCount WRITE setDeviceCount NOTIFY deviceCountChanged)
    Q_PROPERTY(QString taskInProgress READ taskInProgress WRITE setTaskInProgress NOTIFY taskInProgressChanged)
    Q_PROPERTY(QString resolution READ resolution WRITE setResolution NOTIFY resolutionChanged)
    Q_PROPERTY(bool isShowRestartPopup READ isShowRestartPopup WRITE setIsShowRestartPopup NOTIFY isShowRestartPopupChanged)
    Q_PROPERTY(int versionCode READ versionCode WRITE setVersionCode NOTIFY versionCodeChanged)
    Q_PROPERTY(bool appStarted READ appStarted WRITE setAppStarted NOTIFY appStartedChanged)

private:
    explicit AppModel(QObject *parent = nullptr);

public:
    static AppModel* instance();

    bool appStarted() const;
    void setAppStarted(bool data);

    QString ldIntallFolder() const;
    void setLDIntallFolder(const QString path, bool standardPath = false);

    QList<QObject*> devicesList() const;
    void appendDevice(QString instanceName);

    QList<QObject*> devicesRunningList() const;
    void appendRunningDevice(LDIntance* instance);
    void popRunningDevice(LDIntance* instance);

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
    void setMaxNumberThread(int data);

    int maxVMCount() const;
    void setMaxVMCount(int data);

    bool initializing() const;
    void setInitializing(bool data);

    QString currentDir() const;
    void setCurrentDir(QString data);

    QString appName() const;
    void setAppName(QString data);

    bool walletEmpty() const;
    void setWalletEmpty(bool data);

    int deviceCount() const;
    void setDeviceCount(int data);

    QString taskInProgress() const;
    void setTaskInProgress(QString data);

    QString resolution() const;
    void setResolution(QString data);

    bool isShowRestartPopup() const;
    void setIsShowRestartPopup(bool data);

    int versionCode() const;
    void setVersionCode(int data);

    QSize screenResolution() const;
    void setScreenResolution(const QSize& data);

    QString serialNumber() const;
    void setSerialNumber(QString data);

public:
    Q_INVOKABLE void startProgram();
    Q_INVOKABLE void stopProgarm();

signals:
    void ldIntallFolderChanged();
    void devicesListChanged();
    void devicesRunningListChanged();
    void amountOfThreadChanged();
    void isLaunchMutiTaskChanged();
    void tokenChanged();
    void initializingChanged();
    void appNameChanged();
    void walletEmptyChanged();
    void deviceCountChanged();
    void maxNumberThreadChanged();
    void maxVMCountChanged();
    void taskInProgressChanged();
    void resolutionChanged();
    void isShowRestartPopupChanged();
    void versionCodeChanged();
    void appStartedChanged();

    void reInitDeviceList();

    void sigStartProgram();
    void sigStoptProgram();

private:
    static AppModel* m_instance;

    QString m_ldIntallFolder;
    QList<QObject*> m_devicesList;
    QList<QObject*> m_devicesRunningList;
    uint m_amountOfThread;
    bool m_isLaunchMutiTask;
    int m_latestRunningInstance;
    QString m_token;
    APP_CONFIG m_appConfig;
    bool m_initializing;
    QString m_currentDir;
    QString m_appName;
    bool m_walletEmpty;
    int m_deviceCount;
    int m_maxNumberThread;
    int m_maxVMCount;
    QString m_taskInProgress;
    QString m_resolution;
    bool m_isShowRestartPopup;
    int m_versionCode;
    bool m_appStarted;
    QSize m_screenResolution;
    QString m_serialNumber;

public slots:
};

#endif // APPMODEL_H
