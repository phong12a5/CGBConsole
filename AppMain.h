#ifndef APPMAIN_H
#define APPMAIN_H

#include <QObject>
#include <QQuickView>
#include <QQmlContext>
#include "AppDefines.h"
#include "AppModel.h"
#include "AppController.h"
#include <QJsonObject>
#include <QJsonDocument>
#include "EmulatorWorker.h"

class AppMain : public QObject
{
    Q_OBJECT
public:
    explicit AppMain(QObject *parent = nullptr);
    ~AppMain();

    void initApplication();
    void preSetup();

    Q_INVOKABLE void closingApp();
    Q_INVOKABLE int restartApplication();
private:
    QJsonDocument loadJson(QString fileName);
    void saveJson(QJsonDocument document, QString fileName);
    void copyDevices();
    void createTemplateDevice();
    void updateVersion();

private:
    bool m_getAppConfig;
    QThread m_copyDevicesThread;
    QThread m_updateVersionThread;
    QQuickView * m_view;
    EmulatorWorker* m_emulaterWorker;

public slots:
    void onSaveConfig();
    void onLoadConfig();
    void initDevicesList();
    void onStartProgram();
    void onStoptProgram();
    void onFinishCopyDevice(QString deviceName);
    void onFinishCreateTemplateDevice();
    void onUpdateFinished(int code);
signals:
    void startCopyEmulator();
    void startAutoUpdater();
    void startCreateTemplateDevice();
};

#endif // APPMAIN_H
