#ifndef HMAC_H
#define HMAC_H

#include <QObject>
#include <QCryptographicHash>

class HMAC : public QObject
{
    Q_OBJECT
public:
    static QByteArray hmacSha1(QByteArray key, QByteArray baseString);
};

#endif // HMAC_H
