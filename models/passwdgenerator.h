#ifndef PASSWDGENERATOR_H
#define PASSWDGENERATOR_H

#include <QFlag>
#include <QString>
#include <QVector>

class PasswdGenerator
{
public:
    enum SymbolType {
        Lowercase = 0x01,
        Uppercase = 0x02,
        Digits = 0x04,
        Symbols = 0x08,
        Custom = 0x10
    };
    Q_DECLARE_FLAGS(SymbolTypes, SymbolType)
    PasswdGenerator();
    QString generate(int length) const;

    SymbolTypes types() const;
    void setTypes(const SymbolTypes &types);

    void setCustomChars(const QVector<QChar> &customChars);
    void setCustomChars(const QString &customChars);

private:
    SymbolTypes m_types;
    QVector<QChar> m_customChars;
};

#endif // PASSWDGENERATOR_H
