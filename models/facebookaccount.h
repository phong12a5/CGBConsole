#ifndef FACEBOOKACCOUNT_H
#define FACEBOOKACCOUNT_H

#include <QObject>
#include <QJsonObject>
#include <QMutex>
#include <QDateTime>
#include "passwdgenerator.h"

enum Genre{
    MALE = 1,
    FEMALE = 2
};

class FacebookAccount : public QObject
{
    Q_OBJECT
public:
    explicit FacebookAccount(QObject *parent = nullptr,QString username = "",QString password="",QString _2fa="");
    ~FacebookAccount();

    QJsonObject toJsonObject();
    QString toEncodableString();
    bool isInusing();
    void setUsing(bool isUsing);
    bool isDisable();
    void setDisable(bool isDisable);
    void setChecked(bool isChecked);
    bool isChecked();

    QString getHo() const;
    void setHo(const QString &ho);

    QString getTen() const;
    void setTen(const QString &ten);

    QDateTime getBirthDay() const;
    void setBirthDay(const QDateTime &birthDay);

    Genre getGenre() const;
    void setGenre(const Genre &genre);

    QString getProxy() const;
    void setProxy(const QString &proxy);

    QString toString();
    QString toFullString();

    static FacebookAccount* generate();

    static void loadData();

    static FacebookAccount* fromJson(QJsonObject obj);

    void setPassword(const QString &value);

    QString get2fa() const;
    void set2fa(const QString &_2fa);

    QString getUsername() const;
    void setUsername(const QString &value);

    QString getPassword() const;

    bool getIsChangedPass();
    void setIsChangedPass(bool value);

signals:
private:
    QString username;
    QString password;
    QString _2fa;
    QString m_ho;
    QString m_ten;
    QDateTime m_birthDay;
    Genre m_genre;
    QString m_proxy;
    bool isusing = false;
    bool isdisable = false;
    bool ischecked = false;
    bool isChangedPass = false;

    static PasswdGenerator m_passGenerator;

    static QStringList m_firstNameMales;
    static QStringList m_firstNameFemales;
    static QStringList m_lastNames;

    static Genre getRandomGenre();
    static QString getRandomLastName();
    static QString getRandomFirstName(Genre genre);
    static QDateTime getRandomBirthDay(int minAge = 16);
};

#endif // FACEBOOKACCOUNT_H
