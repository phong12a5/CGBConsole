#ifndef QGOOGLEAUTHENTICATOR_H
#define QGOOGLEAUTHENTICATOR_H

#include "qgoogleauthenticator_global.hpp"

#include <QDateTime>
#include <QtEndian>
#include "base32.hpp"
#include "hmac.hpp"

#define SECRET_LENGTH 16
#define VERIFICATION_CODE_MODULUS (1000*1000) // Six digits

class QGOOGLEAUTHENTICATORSHARED_EXPORT QGoogleAuthenticator
{
public:
    static QString getCode(QByteArray secret, quint64 time = 0);
    static bool checkCode(QString code, QByteArray secret);
    static QByteArray generate_secret();
};

#endif // QGOOGLEAUTHENTICATOR_H
