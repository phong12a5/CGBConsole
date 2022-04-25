#include "AppMain.h"
#include <QFile>
#include <QDir>
#include <QFile>
#include <thread>
#include <QCoreApplication>
#include <QApplication>
#include <QStandardPaths>
#include <QScreen>
#include <LDService.h>

#define APP_MODEL   AppModel::instance()
#define APP_CTRL    AppController::instance()
#define WEB_API     WebAPI::instance()

AppMain::AppMain(QObject *parent) :
    QObject(parent)
{

}

AppMain::~AppMain()
{
    LDCommand::instance()->quitAll();
}

void AppMain::initApplication()
{
    LOGD("");
    this->onLoadConfig();
    APP_CTRL->initAppController();
}

void AppMain::onLoadConfig()
{
    LOGD("");
}

void AppMain::closingApp()
{
    LOGD("");
    QCoreApplication::quit();
}

int AppMain::restartApplication()
{
    LOGD("");
    QProcess::startDetached(QApplication::applicationFilePath());
    exit(12);
}

QJsonDocument AppMain::loadJson(QString fileName)
{
    LOGD("");
    QFile jsonFile(fileName);
    jsonFile.open(QFile::ReadOnly);
    return QJsonDocument().fromJson(jsonFile.readAll());
}

void AppMain::saveJson(QJsonDocument document, QString fileName)
{
    LOGD("");
    QFile jsonFile(fileName);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(document.toJson());
}
