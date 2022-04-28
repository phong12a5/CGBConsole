#ifndef ASYNCNETWORKMANAGER_H
#define ASYNCNETWORKMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QElapsedTimer>

#include <QEventLoop>


class AsyncNetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit AsyncNetworkManager(QObject *parent = nullptr);
    ~AsyncNetworkManager();

    QString post(QNetworkRequest request, QString body = "");
    QString get(QNetworkRequest request,QUrl url);
    QString deleteResource(QNetworkRequest request);

signals:
    void networkResponsed(QString result);
private:
    QElapsedTimer timer;
    QEventLoop m_loop;
};

#endif // ASYNCNETWORKMANAGER_H
