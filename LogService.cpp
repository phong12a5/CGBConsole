#include "LogService.h"
#include "AppDefines.h"
#include "WebAPI.hpp"
#include "AppModel.h"
#include <CkHttpRequest.h>
#include <CkHttp.h>
#include <CkHttpResponse.h>
#include <iostream>

#define MODEL AppModel::instance()

LogService* LogService::m_instance = nullptr;

LogService::LogService(QObject *parent) : QObject(parent)
{
    m_thread = new QThread();
    this->moveToThread(m_thread);
    connect(this, &LogService::destroyed, m_thread, &QThread::quit);
    connect(this, &LogService::dologging, this, &LogService::onDoLogging);
    m_thread->start();
}

void LogService::onDoLogging(QString moduleName,QString funcName, QString msg)
{
    LLOGD(QString("-- moduleName: " + moduleName +
                  "-- funcName: " + funcName +
                  "-- msg: " + msg ));

    if(MODEL->token() == ""){
        LLOGD("Missing token");
        return;
    }

    /////////////
    QString url = API_SERVER + QString("DoLogging?token=%1").arg(MODEL->token());
    QUrl serviceUrl = QUrl(url);
    QNetworkRequest request(serviceUrl);
    QJsonObject json;

    json.insert("action", "logging");
    json.insert("appname", MODEL->appName().toLower());
    json.insert("info", "CGBConsole");
    json.insert("module", QString(moduleName));
    json.insert("tag",  QString(funcName));
    json.insert("message",  msg);

    QByteArray jsonData = QJsonDocument(json).toJson();

    CkHttpRequest req;
    CkHttp http;

    CkHttpResponse *resp = nullptr;
    resp = http.PostJson2(url.toLocal8Bit().data(), "application/json", jsonData.data());
    if (resp) delete resp;
}

LogService *LogService::instance()
{
    if(m_instance == nullptr) {
        m_instance = new LogService();
    }
    return m_instance;
}
