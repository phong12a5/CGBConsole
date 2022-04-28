#include "asyncnetworkmanager.h"

#include <QTimer>

AsyncNetworkManager::AsyncNetworkManager(QObject *parent) :
    QObject(parent)
{
//    timer.start();
}

AsyncNetworkManager::~AsyncNetworkManager()
{

}

QString AsyncNetworkManager::post(QNetworkRequest request, QString body)
{
    QNetworkAccessManager *networkManager = new QNetworkAccessManager();
    QNetworkReply *reply = networkManager->post(request,body.toUtf8());
    QEventLoop loop;
    connect(networkManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
    disconnect(networkManager, &QNetworkAccessManager::finished, &m_loop, &QEventLoop::quit);
    QString result = reply->readAll();
    networkManager->deleteLater();
    reply->deleteLater();
    return result;
}

QString AsyncNetworkManager::get(QNetworkRequest request,QUrl url)
{
    /*
        fix if received Ssl TLS failed
        add qtTool OpenSSl x64 or x86 bin path to EnvironmentPath
    */
//    int startGet = timer.elapsed();
    QNetworkAccessManager *networkManager = new QNetworkAccessManager();
    request.setUrl(url);
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    request.setRawHeader("Content-Type","application/json");
    QNetworkReply *reply = networkManager->get(request);
    QEventLoop loop;
    connect(networkManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
    disconnect(networkManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    QString result = reply->readAll();
    networkManager->deleteLater();
    reply->deleteLater();
//    LOG("Finish HTTP GET : %dms",timer.elapsed()-startGet);
    return result;
}
