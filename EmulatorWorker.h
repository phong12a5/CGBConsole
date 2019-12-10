#ifndef EMULATORWORKER_H
#define EMULATORWORKER_H

#include <QObject>
#include "AppDefines.h"

class EmulatorWorker : public QObject
{
    Q_OBJECT
public:
    explicit EmulatorWorker(QObject *parent = nullptr);
    ~EmulatorWorker();

signals:
    void finishCopyDevice(QString deviceName);
    void finishCreateTemplateDevice();

public slots:
    void onCoppyDevices();
    void onCreateTemplateDevice();
};

#endif // EMULATORWORKER_H
