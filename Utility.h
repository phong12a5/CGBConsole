#ifndef UTILITY_H
#define UTILITY_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>

#include <QElapsedTimer>
#include <QImage>
#include <QWindow>
#include <QFileInfo>
#include <QRandomGenerator>

#include <QDebug>

class Utility : public QObject
{
    Q_OBJECT
public:
    explicit Utility(QObject *parent = nullptr);
    static void initImage();

    static void waitForMiliseconds(long milisecs);
    static void writeTo(QString filePath,QString newLines, bool isUTF8 = false);
    static void write(QString filePath,QString content, bool isUTF8 = false);

    static QString read(QString filePath, bool isUTF8 = false);
    static QJsonObject ObjectFromString(const QString &in);

    static QString generateCurrentTime();
    static QString generateCurrentTime2();
    static QString generateDateString();
    static QString getExecutedPath();
    //login
    static int random();
    static QString getRandomNumberArray(int length);
    static QString getRandomString(int length);

    static bool command(QStringList agruments,bool waitForFinish);
    static QString command(QStringList agruments);
    static QJsonArray getFakeLink();
};

#endif // UTILITY_H
