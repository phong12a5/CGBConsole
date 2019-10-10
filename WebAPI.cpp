#include "WebAPI.hpp"
#include "AppModel.h"
#include <QEventLoop>
#include <QFile>

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
        QJsonObject AppInfo =  jsonResponsedObj[MODEL->appName()].toObject();
        config.timeout =    jsonResponsedObj["Timeout"].toString().toInt();
        config.reset_3g =   jsonResponsedObj["Reset3g"].toString().toInt();
        config.debug_mode = jsonResponsedObj["Debug_mode"].toString();
        config.user_type = jsonResponsedObj["User_type"].toString().toInt();
        config.m_ldCount = AppInfo["NoxCount"].toString().toInt();
        config.m_balance = jsonResponsedObj["Balance"].toInt();
        LOG << "config.timeout: " << config.timeout;
        LOG << "config.reset_3g: " << config.reset_3g;
        LOG << "config.debug_mode: " << config.debug_mode;
        LOG << "config.user_type: " << config.user_type;
        LOG << "config.ldCount: " << config.m_ldCount;
        LOG << "config.balance: " << config.m_balance;
        MODEL->setAppConfig(config);
    }
}

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




