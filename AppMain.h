#ifndef APPMAIN_H
#define APPMAIN_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "AppDefines.h"
#include "AppModel.h"
#include "AppController.h"
#include <QJsonObject>
#include <QJsonDocument>
#include "CopyEmulatorWorker.h"

class AppMain : public QObject
{
    Q_OBJECT
public:
    explicit AppMain(QObject *parent = nullptr);
    ~AppMain();

    void initApplication(QQmlApplicationEngine* engine);
    void getConfig();

private:
    QJsonDocument loadJson(QString fileName);
    void saveJson(QJsonDocument document, QString fileName);
    void copyDevices();

private:
    QQmlApplicationEngine* m_engine;
    bool m_getAppConfig;
    QThread m_copyDevicesThread;

public slots:
    void onSaveConfig();
    void onLoadConfig();
    void initDevicesList();
    void onStartProgram();
    void onStoptProgram();
    void onFinishCopyDevice(QString deviceName);

signals:
    void startCopyEmulator();
};

#endif // APPMAIN_H
