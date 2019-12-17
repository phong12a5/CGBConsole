#ifndef LDRUNNER_H
#define LDRUNNER_H

#include <QObject>
#include "AppDefines.h"
#include <QTimer>
#include <LDService.h>

class LDRunner : public QObject
{
    Q_OBJECT
public:
    explicit LDRunner(QString instanceName);
    ~LDRunner();

private:
    QString m_instanceName;
    QTimer *m_checkRunningDevice;
    QTimer *m_checkLifeTcycle;

    int m_deviceStatus;
    int m_appStatus;
    int m_missionStatus;

public:

signals:
    void finished();
    void passConfigToDevice(QString instanceName);

public slots:
    void run();
    void onCheckRunningDevice();
    void onCheckLifeTcycle();

    void onUpdateDeviceStatus(QMap<QString,LDService::E_DEVICE_STATUS> *listDeviceStatus);
    void onUpdateAppStatus(QMap<QString,LDService::E_APP_STATUS> *listAppStatus);
    void onUpdateMissionStatus(QMap<QString,LDService::E_MISSION_STATUS> *listMissionpStatus);
};

#endif // LDRUNNER_H
