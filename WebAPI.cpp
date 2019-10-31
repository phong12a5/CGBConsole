#include "WebAPI.hpp"
#include "AppModel.h"
#include <QEventLoop>
#include <QFile>
if 0
#include <CkJsonObject.h>
#include <CkHttp.h>
#include <CkHttpResponse.h>
#include <CkHttpRequest.h>
#endif
#define MODEL AppModel::instance()

WebAPI* WebAPI::s_instance = nullptr;

WebAPI::WebAPI(QObject *parent) : QObject(parent)
{
    // Do nothing
}

WebAPI *WebAPI::instance()
{
    if(s_instance == nullptr){
        s_instance = new WebAPI();
    }
    return s_instance;
}

void WebAPI::getConfig()
{
    QString url = API_SERVER + QString("config?token=%1").arg(MODEL->token());
    QUrl serviceUrl = QUrl(url);
    QNetworkRequest request(serviceUrl);
    QJsonObject json;

    json.insert("action", QTextCodec::codecForMib(106)->toUnicode(getEncodedString("config")));
    json.insert("appname", QTextCodec::codecForMib(106)->toUnicode(getEncodedString(MODEL->appName().toLower())));
    json.insert("info", QTextCodec::codecForMib(106)->toUnicode(getEncodedDeviceInfo()));

    QByteArray jsonData = QJsonDocument(json).toJson();
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader,QByteArray::number(jsonData.size()));

    QEventLoop evenlopp;
    QNetworkAccessManager* networkManager = new QNetworkAccessManager(this);
    QObject::connect(networkManager, SIGNAL(finished(QNetworkReply*)),
                     &evenlopp, SLOT(quit()));
    QNetworkReply* reply = networkManager->post(request, jsonData);
    evenlopp.exec();

    QByteArray responseData = reply->readAll();
    QJsonObject jsonObj = QJsonDocument::fromJson(responseData).object();

    if(jsonObj.isEmpty()){
        LOG << "jsonObj is empty!";
        return;
    }else{
        // Continue
    }

    QString data =  jsonObj["data"].toString();
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
    QByteArray decodeText = encryption.decode(QByteArray::fromBase64(data.toUtf8()), getKey().toLocal8Bit(), getIV().toLocal8Bit());
    QJsonDocument jdoc = QJsonDocument::fromJson(encryption.removePadding(decodeText));
    QJsonObject jsonResponsedObj = jdoc.object();
    LOG << "jsonResponsedObj: " << jsonResponsedObj;
    if(!jsonResponsedObj.isEmpty()){
        APP_CONFIG config;
        config.timeout =                jsonResponsedObj["timeout"].toString().toInt();
        config.reset_3g =               jsonResponsedObj["reset3g"].toString().toInt();
        config.debug_mode =             jsonResponsedObj["debug_mode"].toString();
        config.user_type =              jsonResponsedObj["user_type"].toString().toInt();
        config.m_maxVmCount =           jsonResponsedObj["totalvm"].toString().toInt();
        config.m_maxVmThread =          jsonResponsedObj["maxthreadnoxld"].toString().toInt();
        config.m_balance =              jsonResponsedObj["balance"].toInt();
        config.m_openApkAfterNSeconds = jsonResponsedObj["openapkafternseconds"].toString().toInt();
        LOG << "config.timeout: " << config.timeout;
        LOG << "config.reset_3g: " << config.reset_3g;
        LOG << "config.debug_mode: " << config.debug_mode;
        LOG << "config.user_type: " << config.user_type;
        LOG << "config.m_maxVmCount: " << config.m_maxVmCount;
        LOG << "config.m_maxVmThread: " << config.m_maxVmThread;
        LOG << "config.balance: " << config.m_balance;
        LOG << "config.m_openApkAfterNSeconds: " << config.m_openApkAfterNSeconds;
        MODEL->setAppConfig(config);
    }
}

#if 0
void WebAPI::getConfig()
{
    QString url = API_SERVER + QString("config?token=%1").arg(MODEL->token());
    QUrl serviceUrl = QUrl(url);
    QNetworkRequest request(serviceUrl);
    QJsonObject json;

    json.insert("action", QTextCodec::codecForMib(106)->toUnicode(getEncodedString("config")));
    json.insert("appname", QTextCodec::codecForMib(106)->toUnicode(getEncodedString(MODEL->appName().toLower())));
    json.insert("info", QTextCodec::codecForMib(106)->toUnicode(getEncodedDeviceInfo()));

    QByteArray jsonData = QJsonDocument(json).toJson();

    CkHttpRequest req;
    CkHttp http;

    CkHttpResponse *resp = nullptr;
    resp = http.PostJson2(url.toLocal8Bit().data(), "application/json", jsonData.data());
    if (http.get_LastMethodSuccess() != true) {
        LOG << "Http error: " << QString(http.lastErrorText());
    } else {
        if (resp->bodyStr()) {
            LOG << "Reponsed: " << QString(resp->bodyStr());
            CkJsonObject jsonResponse;
            bool loadJson = jsonResponse.Load(resp->bodyStr());
            if (loadJson) {
                QString data = QString(jsonResponse.stringOf("data"));
                QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
                QByteArray decodeText = encryption.decode(QByteArray::fromBase64(data.toUtf8()), getKey().toLocal8Bit(), getIV().toLocal8Bit());
                QJsonDocument jdoc = QJsonDocument::fromJson(encryption.removePadding(decodeText));
                QJsonObject jsonResponsedObj = jdoc.object();
                LOG << "jsonResponsedObj: " << jsonResponsedObj;
                if(!jsonResponsedObj.isEmpty()){
                    APP_CONFIG config;
                    config.timeout = jsonResponsedObj["timeout"].toString().toInt();
                    config.reset_3g = jsonResponsedObj["reset3g"].toString().toInt();
                    config.debug_mode = jsonResponsedObj["debug_mode"].toString();
                    config.user_type = jsonResponsedObj["user_type"].toString().toInt();
                    config.m_maxVmCount = jsonResponsedObj["totalvm"].toString().toInt();
                    config.m_maxVmThread = jsonResponsedObj["maxthreadnoxld"].toString().toInt();
                    config.m_balance = jsonResponsedObj["balance"].toInt();
                    config.m_openApkAfterNSeconds = jsonResponsedObj["openapkafternseconds"].toString().toInt();
                    LOG << "config.timeout: " << config.timeout;
                    LOG << "config.reset_3g: " << config.reset_3g;
                    LOG << "config.debug_mode: " << config.debug_mode;
                    LOG << "config.user_type: " << config.user_type;
                    LOG << "config.m_maxVmCount: " << config.m_maxVmCount;
                    LOG << "config.m_maxVmThread: " << config.m_maxVmThread;
                    LOG << "config.balance: " << config.m_balance;
                    LOG << "config.m_openApkAfterNSeconds: " << config.m_openApkAfterNSeconds;
                    MODEL->setAppConfig(config);
                }

            } else {
                LOG << "Could not load resp->bodyStr() -> JsonObject";
            }
        } else {
            LOG << "resp->bodyStr() is NULL";
        }
    }
}
#endif

QString WebAPI::getKey() const
{
    return "Congaubeo@123456Congaubeo@123456";
}

QString WebAPI::getIV() const
{
    return "0123456789012345";
}

QByteArray WebAPI::getEncodedDeviceInfo() const
{
    QJsonObject deviceInfo;
    deviceInfo["DeviceInfo"] = "CGBConsole";
    QByteArray deviceInfoData = QJsonDocument(deviceInfo).toJson();
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
    return encryption.encode(deviceInfoData, getKey().toLocal8Bit(), getIV().toLocal8Bit()).toBase64();
}

QByteArray WebAPI::getEncodedString(QString action) const
{
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
    return encryption.encode(action.toLocal8Bit(), getKey().toLocal8Bit(), getIV().toLocal8Bit()).toBase64();
}

bool WebAPI::downloadFIle(QString url, QString savedPath)
{
    bool status = false;
    QEventLoop evenlopp;
    QNetworkAccessManager* networkManager = new QNetworkAccessManager();
    QObject::connect(networkManager, SIGNAL(finished(QNetworkReply*)),
                     &evenlopp, SLOT(quit()));
    QNetworkReply* reply = networkManager->get(QNetworkRequest(QUrl(url)));
    evenlopp.exec();

    QNetworkReply::NetworkError error = reply->error();
    if(error != QNetworkReply::NoError){
        status = false;
        LOG << "Error: " << reply->errorString();
    }else{
        QByteArray content = reply->readAll();
        if(content.isEmpty())
        {
            LOG << "Responed content is empty";
        }else{
            QFile *file = new QFile(savedPath);
            if(file->open(QIODevice::WriteOnly))
            {
                file->write(content);
                file->flush();
                file->close();
                status = true;
            }else{
                status = false;
                LOG << "Failed to open file";
            }
            delete file;
        }
    }
    return status;
}




