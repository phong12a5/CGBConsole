#ifndef EMULATORWORKER_H
#define EMULATORWORKER_H

#include <QObject>
#include "AppDefines.h"

class EmulatorWorker : public QObject
{
    Q_OBJECT
public:
    enum E_CREATE_TEMPDEVICE_STATUS: int {
        E_CREATE_TEMPDEVICE_SUCESS = 0,
        E_CREATE_TEMPDEVICE_CREATE_TEMP_FAIL,
        E_CREATE_TEMPDEVICE_GET_APK_FAIL,
        E_CREATE_TEMPDEVICE_INSTALL_APK_FAIL,
        E_CREATE_TEMPDEVICE_PASS_DATABASE_FAIL
    };

    explicit EmulatorWorker(QObject *parent = nullptr);
    ~EmulatorWorker();

private:
    bool extractDatabases();

signals:
    void finishCopyDevice(QString deviceName);
    void finishCreateTemplateDevice(int);
    void finishCopyTask();

public slots:
    void onCoppyDevices();
    void onCreateTemplateDevice();
};

#endif // EMULATORWORKER_H
