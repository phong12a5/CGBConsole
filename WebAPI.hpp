#ifndef WEBAPI_H
#define WEBAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "QAESEncryption.hpp"
#include <QTextCodec>
#include <QStandardPaths>

class WebAPI : public QObject
{
    Q_OBJECT

private:
    explicit WebAPI(QObject *parent = nullptr);
    bool unlockChilkat();

public:
    static WebAPI *instance();

private:
    static WebAPI* s_instance;

private:

    QString getKey() const;
    QString getIV() const;
    QByteArray getEncodedDeviceInfo() const;
    QByteArray getEncodedString(QString action) const;

public:
    void getConfig();
    bool downloadFIle(QString url, QString savedPath);
    bool downloadApk(int version);

public slots:

signals:
};

#endif // WEBAPI_H
