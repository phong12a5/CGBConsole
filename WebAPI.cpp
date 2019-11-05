#include "WebAPI.hpp"
#include "AppModel.h"
#include <QEventLoop>
#include <QFile>
#include <CkJsonObject.h>
#include <CkHttp.h>
#include <CkHttpResponse.h>
#include <CkHttpRequest.h>
#include <CkGlobal.h>
#include <CkRest.h>
#include <CkStream.h>

#define MODEL AppModel::instance()

WebAPI* WebAPI::s_instance = nullptr;

WebAPI::WebAPI(QObject *parent) : QObject(parent)
{
    // Do nothing
    if (unlockChilkat()){
        LOG << "unlockChilkat successfully";
    } else {
        LOG << "unlockChilkat Failure";
    }
}

WebAPI *WebAPI::instance()
{
    if(s_instance == nullptr){
        s_instance = new WebAPI();
    }
    return s_instance;
}

bool WebAPI::unlockChilkat() {
    LOG << "unlockChilkat";
    CkGlobal glob;
    bool success_global = glob.UnlockBundle("VONGTH.CB4082020_9kru5rnD5R2h");
    if (success_global != true) {
        LOG << "Error: " << QString(glob.lastErrorText());
        return false;
    }

    int status = glob.get_UnlockStatus();
    if (status == 2) {
        LOG << "Unlocked using purchased unlock code.";
    } else {
        LOG <<"Unlocked in trial mode.";
    }


    return true;
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
                    config.m_android_versioncode = jsonResponsedObj["android_versioncode"].toString().toInt();
                    config.m_dropboxaccesstoken = jsonResponsedObj["dropboxaccesstoken"].toString();
                    LOG << "config.timeout: " << config.timeout;
                    LOG << "config.reset_3g: " << config.reset_3g;
                    LOG << "config.debug_mode: " << config.debug_mode;
                    LOG << "config.user_type: " << config.user_type;
                    LOG << "config.m_maxVmCount: " << config.m_maxVmCount;
                    LOG << "config.m_maxVmThread: " << config.m_maxVmThread;
                    LOG << "config.balance: " << config.m_balance;
                    LOG << "config.openApkAfterNSeconds: " << config.m_openApkAfterNSeconds;
                    LOG << "config.android_versioncode: " << config.m_android_versioncode;
                    LOG << "config.dropboxaccesstoken: " << config.m_dropboxaccesstoken;
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

bool WebAPI::downloadApk(int version) {
    LOG << "downloadApk -> Version: " << version;
    CkRest rest;

    //  Connect to Dropbox
    bool success = rest.Connect("content.dropboxapi.com", 443, true, true);
    if (success != true) {
        LOG << "Connect error: " << QString(rest.lastErrorText());
        return success;
    }

    //  Add request headers.
    QString tokenStr = "Bearer " + AppModel::instance()->appConfig().m_dropboxaccesstoken;
    LOG << "Token: " << tokenStr;
    rest.AddHeader("Authorization", tokenStr.toLocal8Bit().data());

    QJsonObject json;
    QString clouldPathStr = "/apk/xyz.autofarmer.app." + QString::number(version) + ".apk";
    LOG << "clouldPathStr: " << clouldPathStr;
    json["path"] = clouldPathStr;
    rest.AddHeader("Dropbox-API-Arg", QJsonDocument(json).toJson().data());

    QString localPathStr = "xyz.autofarmer.app." + QString::number(version) + ".apk";
    LOG << "localPathStr: " << localPathStr;
    CkStream fileStream;
    fileStream.put_SinkFile(localPathStr.toLocal8Bit().data());

    int expectedStatus = 200;
    rest.SetResponseBodyStream(expectedStatus, true, fileStream);

    const char *responseStr = rest.fullRequestNoBody("POST", "/2/files/download");
    if (rest.get_LastMethodSuccess() != true) {
        LOG << "responseStr error: " << QString(rest.lastErrorText());
        return false;
    } else {
        LOG << "responseStr: " << QString(responseStr);
    }

    //  When successful, Dropbox responds with a 200 response code.
    if (rest.get_ResponseStatusCode() != 200) {
        //  Examine the request/response to see what happened.
        LOG << "response status code = " << QString(rest.get_ResponseStatusCode());
        LOG << "response status text = " << QString(rest.responseStatusText());
        LOG << "response header: " << QString(rest.responseHeader());
        LOG << "response body (if any): " << QString(responseStr);
        LOG << "LastRequestStartLine: " << QString(rest.lastRequestStartLine());
        LOG << "LastRequestHeader: " << QString(rest.lastRequestHeader());
        return false;
    }
    LOG << "Download successful";
    return true;
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




