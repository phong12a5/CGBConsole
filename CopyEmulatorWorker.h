#ifndef COPYEMULATOR_H
#define COPYEMULATOR_H

#include <QObject>
#include "AppDefines.h"

class CopyEmulatorWorker : public QObject
{
    Q_OBJECT
public:
    explicit CopyEmulatorWorker(QObject *parent = nullptr);
    ~CopyEmulatorWorker();

signals:
    void finishCopyDevice(QString deviceName);

public slots:
    void doWork();
};

#endif // COPYEMULATOR_H
