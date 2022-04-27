#ifndef CONFIGHELPER_H
#define CONFIGHELPER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QDir>
#include <QFile>

#include "utility.h"

class ConfigHelper : public QObject
{
    Q_OBJECT
public:
    explicit ConfigHelper(QObject *parent = nullptr);

    static void setLDPlayerLibsPath(QString path);
    static void setNumThread(int numThread);
    static void setAccountPath(QString path);
    static void setFacebookApkPath(QString apkPath);
    static void setAdbKeyboardApkPath(QString apkPath);
    static void setScreenLogPath(QString screenLogPath);
    static void setPackagePath(QString packagePath);
    static void setAutoStart(bool isAutoStart);
    static void setVipLikePost(bool isVipLikePost);
    static void setRegClone(bool isRegClone);
    static void setBuffLike(bool isBuffLike);
    static void setBuffComment(bool isBuffComment);
    static void setLikePage(bool isLikePage);
    static void setFollow(bool isFollow);

    static QString GetLDPlayerLibsPath();
    static int getNumThread();
    static QString getAccountPath();
    static int getConfigVersion();
    static QString getFacebookApkPath();
    static QString getAdbKeyboardApkPath();
    static QString getScreenLogPath();
    static QString getPackagePath();
    static bool isAutoStart();
    static bool isVipLikePost();
    static bool isRegClone();
    static bool isBuffLike();
    static bool isBuffComment();
    static bool isLikePage();
    static bool isFollow();
    static int getReleaseVersion();

    static void initConfig();
private:
    static QString configPath;

    static QString configVersion;
    static int currentVersion;
    static QString configKey;

    static QString numThreadKey;
    static QString ldPlayerLibsKey;
    static QString accountPathKey;
    static QString facebookApkPathKey;
    static QString adbKeyboardApkPathKey;
    static QString screenLogPathKey;
    static QString packagePathKey;
    static QString autoStartKey;
    static QString vipLikePostModeKey;
    static QString regCloneModeKey;
    static QString buffLikeModeKey;
    static QString buffCommentModeKey;
    static QString likePageModeKey;
    static QString followModeKey;


    static int releaseVersion;

//    static QString enable2faKey;
//    static QString tuongtacKey;
//    static QString loginKey;
//    static QString dangbaiKey;


    static QJsonObject getConfig();
    static void updateConfig(QJsonObject object);
};

#endif // CONFIGHELPER_H
