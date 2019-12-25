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
#include <CkZip.h>

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

bool WebAPI::getConfig()
{
    bool success = false;
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
        LOGD("Http error: " + QString(http.lastErrorText()));
    } else {
        if (resp->bodyStr()) {
            CkJsonObject jsonResponse;
            bool loadJson = jsonResponse.Load(resp->bodyStr());
            if (loadJson) {
                QString data = QString(jsonResponse.stringOf("data"));
                QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
                QByteArray decodeText = encryption.decode(QByteArray::fromBase64(data.toUtf8()), getKey().toLocal8Bit(), getIV().toLocal8Bit());
                QJsonDocument jdoc = QJsonDocument::fromJson(encryption.removePadding(decodeText));
                QJsonObject jsonResponsedObj = jdoc.object();
                LOGD(QJsonDocument(jsonResponsedObj).toJson(QJsonDocument::Compact));
                if(!jsonResponsedObj.isEmpty() && jsonResponsedObj["updated"] != "failed"){
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
                    config.m_android_testversioncode = jsonResponsedObj["apktestversion"].toString().toInt();
                    config.m_dropboxaccesstoken = jsonResponsedObj["dropboxaccesstoken"].toString();
                    config.m_cgbconsole_versioncode = jsonResponsedObj["cgbconsole_versioncode"].toString().toInt();
                    LOGD("config.timeout: " + QString::number(config.timeout));
                    LOGD("config.reset_3g: "   + QString::number(config.reset_3g));
                    LOGD("config.debug_mode: "  + config.debug_mode);
                    LOGD("config.user_type: "  + QString::number( config.user_type));
                    LOGD("config.m_maxVmCount: "  + QString::number( config.m_maxVmCount));
                    LOGD("config.m_maxVmThread: "  + QString::number( config.m_maxVmThread));
                    LOGD("config.balance: "  + QString::number( config.m_balance));
                    LOGD("config.openApkAfterNSeconds: "  + QString::number( config.m_openApkAfterNSeconds));
                    LOGD("config.android_versioncode: "  + QString::number( config.m_android_versioncode));
                    LOGD("config.android_testversioncode: "  + QString::number(config.m_android_testversioncode));
                    LOGD("config.dropboxaccesstoken: "  + config.m_dropboxaccesstoken);
                    LOGD("config.cgbconsole_versioncode: "  + QString::number(config.m_cgbconsole_versioncode));
                    MODEL->setAppConfig(config);
                    success = true;
                }

            } else {
                LOGD("Could not load resp->bodyStr() -> JsonObject");
            }
        } else {
            LOGD("resp->bodyStr() is NULL");
        }
    }
    return success;
}

bool WebAPI::downloadApk(int version, bool testMode) {
    LOGD("downloadApk -> Version: " +  QString::number(version));
    CkRest rest;

    //  Connect to Dropbox
    bool success = rest.Connect("content.dropboxapi.com", 443, true, true);
    if (success != true) {
        LOGD("Connect error: " + QString(rest.lastErrorText()));
        return success;
    }

    //  Add request headers.
    QString tokenStr = "Bearer " + AppModel::instance()->appConfig().m_dropboxaccesstoken;
    LOGD("Token: " + tokenStr);
    rest.AddHeader("Authorization", tokenStr.toLocal8Bit().data());

    QJsonObject json;
    QString clouldPathStr;
    if(testMode)
        clouldPathStr = "/apk/xyz.autofarmer.app." + QString::number(version) + ".test.apk";
    else
        clouldPathStr = "/apk/xyz.autofarmer.app." + QString::number(version) + ".apk";

    LOGD("clouldPathStr: " + clouldPathStr);
    json["path"] = clouldPathStr;
    rest.AddHeader("Dropbox-API-Arg", QJsonDocument(json).toJson().data());

    QString localPathStr = "xyz.autofarmer.app." + QString::number(version) + ".apk";
    if(testMode)
        localPathStr = "xyz.autofarmer.app." + QString::number(version) + ".test.apk";
    else
        localPathStr = "xyz.autofarmer.app." + QString::number(version) + ".apk";
    LOGD("localPathStr: " + localPathStr);
    CkStream fileStream;
    fileStream.put_SinkFile(localPathStr.toLocal8Bit().data());

    int expectedStatus = 200;
    rest.SetResponseBodyStream(expectedStatus, true, fileStream);

    const char *responseStr = rest.fullRequestNoBody("POST", "/2/files/download");
    if (rest.get_LastMethodSuccess() != true) {
        LOGD("responseStr error: " + QString(rest.lastErrorText()));
        return false;
    } else {
        LOGD("responseStr: " + QString(responseStr));
    }

    //  When successful, Dropbox responds with a 200 response code.
    if (rest.get_ResponseStatusCode() != 200) {
        //  Examine the request/response to see what happened.
        LOGD("response status code = " + QString(rest.get_ResponseStatusCode()));
        LOGD("response status text = " + QString(rest.responseStatusText()));
        LOGD("response header: " + QString(rest.responseHeader()));
        LOGD("response body (if any): " + QString(responseStr));
        LOGD("LastRequestStartLine: " + QString(rest.lastRequestStartLine()));
        LOGD("LastRequestHeader: " + QString(rest.lastRequestHeader()));
        return false;
    }
    LOGD("Download successful");
    return true;
}

bool WebAPI::downloadNewVersion()
{
    LOGD("");
    CkRest rest;
    //  Connect to Dropbox
    bool success = rest.Connect("content.dropboxapi.com", 443, true, true);
    if (success != true) {
        LOGD("Connect error: " + QString(rest.lastErrorText()));
        return success;
    }

    //  Add request headers.
    QString tokenStr = "Bearer " + AppModel::instance()->appConfig().m_dropboxaccesstoken;
    LOGD("Token: " + tokenStr);
    rest.AddHeader("Authorization", tokenStr.toLocal8Bit().data());

    QJsonObject json;
    QString clouldPathStr = "/CGBConsole/CGBConsole.zip";
    LOGD("clouldPathStr: " + clouldPathStr);
    json["path"] = clouldPathStr;
    rest.AddHeader("Dropbox-API-Arg", QJsonDocument(json).toJson().data());

    QString localPathStr = "CGBConsole.zip";
    LOGD("localPathStr: " + localPathStr);
    CkStream fileStream;
    fileStream.put_SinkFile(localPathStr.toLocal8Bit().data());

    int expectedStatus = 200;
    rest.SetResponseBodyStream(expectedStatus, true, fileStream);

    const char *responseStr = rest.fullRequestNoBody("POST", "/2/files/download");
    if (rest.get_LastMethodSuccess() != true) {
        LOGD("responseStr error: " + QString(rest.lastErrorText()));
        return false;
    } else {
        LOGD("responseStr: " + QString(responseStr));
    }

    //  When successful, Dropbox responds with a 200 response code.
    if (rest.get_ResponseStatusCode() != 200) {
        //  Examine the request/response to see what happened.
        LOGD("response status code = " + QString(rest.get_ResponseStatusCode()));
        LOGD("response status text = " + QString(rest.responseStatusText()));
        LOGD("response header: " + QString(rest.responseHeader()));
        LOGD("response body (if any): " + QString(responseStr));
        LOGD("LastRequestStartLine: " + QString(rest.lastRequestStartLine()));
        LOGD("LastRequestHeader: " + QString(rest.lastRequestHeader()));
        return false;
    }
    return true;
}

void WebAPI::dologging(const char *moduleName, const char *funcName, QString msg)
{
    if(MODEL->token() == ""){
        LLOGD("Missing toke");
        return;
    }

//    std::string url = std::string(API_SERVER) + std::string("DoLogging?token=") + std::string(AppModel::instance()->token());
//    CkJsonObject jsonReqBody;
//    jsonReqBody.AddStringAt(-1, "action", "logging");
//    jsonReqBody.AddStringAt(-1, "appname", "");
//    jsonReqBody.AddStringAt(-1, "info", "");
//    jsonReqBody.AddStringAt(-1, "module", moduleName);
//    jsonReqBody.AddStringAt(-1, "tag", funcName);
//    jsonReqBody.AddStringAt(-1, "message", msg);
//    CkHttpRequest req;
//    CkHttp http;
//    CkHttpResponse *resp = nullptr;
//    resp = http.PostJson2(url.c_str(), "application/json", jsonReqBody.emit());
//    if (resp) delete resp;

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
    LLOGD("Error: " + QString(resp->lastErrorText()));
    if (resp) delete resp;
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
        LOGD("Error: " + reply->errorString());
    }else{
        QByteArray content = reply->readAll();
        if(content.isEmpty())
        {
            LOGD("Responed content is empty");
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
                LOGD("Failed to open file");
            }
            delete file;
        }
    }
    return status;
}




