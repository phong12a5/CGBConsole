#ifndef LDSERVICE_H
#define LDSERVICE_H

#include <QObject>
#include <QTimer>
#include <QMap>

class LDService : public QObject
{
    Q_OBJECT
public:

    enum E_DEVICE_STATUS: int {
        E_DEVICE_DISCONNECT = 0,
        E_DEVICE_CONNECTED
    };

    enum E_APP_STATUS: int {
        E_APP_STOPPED = 0,
        E_APP_RUNNING
    };

    enum E_MISSION_STATUS: int {
        E_MISSION_INCOMPLETED = 0,
        E_MISSION_COMPLETED
    };

private:
    explicit LDService(QObject *parent = nullptr);
    ~LDService();

public:
    static LDService* instance();

signals:
    void updateDeviceStatus(QMap<QString,E_DEVICE_STATUS> *listDeviceStatus);
    void updateAppStatus(QMap<QString,E_APP_STATUS> *listAppStatus);
    void updateMissionStatus(QMap<QString,E_MISSION_STATUS> *listMissionpStatus);

public slots:
    void startService();
    void stopService();

    void onCheckDeviceStatus();
    void onCheckRunApp();
    void onCheckMissionStatus();

    void onPassConfigToDevice(QString instanceName);

private:
    static LDService* m_instance;

    QTimer* m_checkConnectTimer;
    QTimer* m_checkRunAppTimer;
    QTimer* m_checkMissionSttTimer;

    QMap<QString,E_DEVICE_STATUS> * m_listDeviceStatus;
    QMap<QString,E_APP_STATUS> * m_listAppStatus;
    QMap<QString,E_MISSION_STATUS> * m_listMissionStatus;
};

Q_DECLARE_METATYPE(LDService::E_DEVICE_STATUS);
Q_DECLARE_METATYPE(LDService::E_APP_STATUS);
Q_DECLARE_METATYPE(LDService::E_MISSION_STATUS);

#endif // LDSERVICE_H
