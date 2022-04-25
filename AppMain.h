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
    Q_INVOKABLE void startProgram();
    Q_INVOKABLE void stopProgarm();

private:
    QJsonDocument loadJson(QString fileName);
    void saveJson(QJsonDocument document, QString fileName);

public slots:
    void onLoadConfig();
};

#endif // APPMAIN_H
