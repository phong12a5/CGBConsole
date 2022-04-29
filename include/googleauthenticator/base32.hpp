#ifndef BASE32_H
#define BASE32_H

#include <QString>

class Base32
{
public:
    static int base32_encode(QString *data, QString *result, int bufferSize);
    static int base32_decode(const quint8 *encoded, quint8 *result, int bufSize);
};

#endif // BASE32_H
