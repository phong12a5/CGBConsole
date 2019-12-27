#include "DownloadService.h"
#include <CkRest.h>
#include <CkStringBuilder.h>
#include <CkJsonObject.h>
#include <AppModel.h>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <WebAPI.hpp>
#include <CkZip.h>

#define APP_MODEL   AppModel::instance()
#define WEB_API     WebAPI::instance()

DownloadService::DownloadService(QObject *parent) : QObject(parent)
{
    m_thread = new QThread();
    this->moveToThread(m_thread);
    connect(this, &DownloadService::destroyed, m_thread, &QThread::quit);
    connect(this, &DownloadService::startService, this, &DownloadService::run);
    m_thread->start();
}

void DownloadService::run()
{
    LOGD("");
    this->downloadTrainedData("eng");
    this->downloadApp();
}

QMap<QString, QString> DownloadService::getListContentOfFolder(QString folderPath) {
    LOGD(folderPath);
    QMap<QString, QString> result;
    result.clear();
    CkRest rest;
    bool bTls = true;
    int port = 443;
    bool bAutoReconnect = true;
    if (rest.Connect("api.dropboxapi.com",port,bTls,bAutoReconnect) != true) {
        LOGD(QString("ConnectFailReason: %1").arg(rest.get_ConnectFailReason()));
        LOGD(QString("Error: %1").arg(rest.lastErrorText()));
        return result;
    }

    //  See the Online Tool for Generating JSON Creation Code
    CkJsonObject json;
    json.UpdateString("path",folderPath.toLocal8Bit().data());
    json.UpdateBool("recursive",false);
    json.UpdateBool("include_media_info",false);
    json.UpdateBool("include_deleted",false);
    json.UpdateBool("include_has_explicit_shared_members",false);
    json.UpdateBool("include_mounted_folders",true);

    QString tokenStr = "Bearer " + APP_MODEL->appConfig().m_dropboxaccesstoken;
    rest.AddHeader("Authorization", tokenStr.toLocal8Bit().data());
    rest.AddHeader("Content-Type","application/json");

    CkStringBuilder sbRequestBody;
    json.EmitSb(sbRequestBody);
    CkStringBuilder sbResponseBody;
    if (rest.FullRequestSb("POST","/2/files/list_folder",sbRequestBody,sbResponseBody) != true) {
        LOGD(QString("rest.FullRequestSb: ")  + rest.lastErrorText());
        return result;
    }

    int respStatusCode = rest.get_ResponseStatusCode();
    if (respStatusCode >= 400) {
        LOGD("Response Status Code = " + QString::number(respStatusCode));
        LOGD("Response Header: " + QString(rest.responseHeader()));
        LOGD("Response Body: " + QString(sbResponseBody.getAsString()));
        return result;
    }

    CkJsonObject jsonResponse;
    jsonResponse.LoadSb(sbResponseBody);

    //  See the Online Tool for Generating JSON Parse Code
    int i;
    int count_i;

    i = 0;
    count_i = jsonResponse.SizeOfArray("entries");
    while (i < count_i) {
        jsonResponse.put_I(i);
        const char *Tag = jsonResponse.stringOf("entries[i].\".tag\"");
        const char *name = jsonResponse.stringOf("entries[i].name");
        result.insert(QString(name),QString(Tag));
        i = i + 1;
    }
    qDebug() << "result: " << result;
    return result;
}

bool DownloadService::downloadTrainedData(QString langCode)
{
    LOGD(langCode);
    bool success = false;
    QString fileName = langCode + ".traineddata.zip";
    QString unzippedFileName = langCode + ".traineddata";
    QString cloudPath = "/languages/" + fileName;
    QString langFolderPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/LDPlayer/Applications/" + FARM_PACKAGE_NAME + "/languages/";
    QString localPath = langFolderPath + fileName;
    QString unzippedLocalPath = langFolderPath + unzippedFileName;
    QDir().mkpath(langFolderPath);

    QMap<QString, QString> listSub = this->getListContentOfFolder("/languages");
    if(listSub.contains(fileName)){
        if(listSub.value(fileName) == "file"){
            if(QFile(unzippedLocalPath).exists() == false){
                if(WEB_API->downloadFileFromDropbox(APP_MODEL->appConfig().m_dropboxaccesstoken,
                                                    cloudPath,
                                                    localPath)){
                    LOGD("Download " + fileName + " succesfully");
                    CkZip zip;
                    if (zip.OpenZip(localPath.toLocal8Bit().data()) != true) {
                        LOGD(QString("zip.lastErrorText: ") + zip.lastErrorText());
                    }else {
                        int unzipCount;
                        unzipCount = zip.Unzip(langFolderPath.toLocal8Bit().data());
                        if (unzipCount < 0) {
                            LOGD(QString("zip.lastErrorText: ") + zip.lastErrorText());
                        } else {
                            LOGD("Extracted " + fileName + " succesfully");
                            success = true;
                        }
                        zip.CloseZip();
                    }
                }
            }else {
                LOGD(fileName + " existed already");
            }
            QFile::remove(localPath);
        }else {
            LOGD(fileName + " doesn't existed in cloud");
        }
    }else {
        LOGD(fileName + " doesn't existed in cloud");
    }
    return success;
}

void DownloadService::downloadApp()
{
    QMap<QString, QString> listSub = this->getListContentOfFolder("/" + APP_MODEL->appName().toLower() + "/simulator");
    QStringList keys = listSub.keys();
    QString apkfolderName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/LDPlayer/Applications/" + FARM_PACKAGE_NAME + "/" + APP_MODEL->appName().toLower();
    QDir().mkpath(apkfolderName);

    foreach(QString item, keys){
        if(listSub.value(item) == "file"){
            QString itemPath = apkfolderName + "/" + item;
            LOGD("itemPath: " + itemPath);
            if(QFile(itemPath).exists() == false){
                if(WEB_API->downloadFileFromDropbox(APP_MODEL->appConfig().m_dropboxaccesstoken,
                                                    "/" + APP_MODEL->appName().toLower() + "/simulator" + "/" + item,
                                                    itemPath)){
                    LOGD("Download " + item + " succesfully");
                }
            }else {
                    LOGD(item + " existed already");
            }
        }
    }
}
