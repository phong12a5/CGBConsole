#ifndef LDPLAYER_H
#define LDPLAYER_H

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QDateTime>

#include "LDPlayerHelper.h"
#include "DefinedScreen.h"

#define INVALID_POINT QPoint(-1,-1)
#define LOG qDebug()<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")<<"ID:"<<profile.id<<__FUNCTION__<<__LINE__

enum PropertyType{
    CLASS_NAME = 1,
    CONTENT_DESCRIPTION = 2,
    TEXT = 3
};


class LDPlayer : public QObject
{
    Q_OBJECT

public:
    explicit LDPlayer(int dncsId);
    ~LDPlayer();
    //start
    void start();
//    void modifyResolution(DeviceInfo &deviceInfo);
    QString getManufacturner();
    QString getModel();
    void disableGPS();
    void enableAccessibility(QString packageName, QString serviceName);
    void disableAccessibilityService();

    bool isRunning();
    bool isAdbDeviceAvailable();
    bool refreshAdbAddress();
    bool isWindowAvailable();
    void setupProxy();
    void clearProxy();
    QString getCurrentProxy();
    void openDeepLink(QString deepLink);
    bool restoreLDPlayer(QString filePath);
    bool fakeDevice();
    QString getXposedData();
    void restoreDeviceInfo(QJsonObject xposedData);

    //general
    void setupKeyboard(bool isUnicode = false);
    void tapOn(double x,double y);
    void tapOn(QPoint point);
    void doubleTapOn(double x, double y);
    void swipeUp(bool isLong = false);
    void swipDown(bool isLong = false);
    void inputText(QString content, bool isUnicode = false, bool isSlow = false);
    void inputKeyEvent(QString key);
    void clearText(bool isClearAll = false);

    //login functions
    QString backupFacebook(QString uuid, QString destinationPath);
    bool restoreFacebook(QString sourcePath);
    bool openPackage(QString packageName,bool isCleanUp = false);
    bool isInstallApp(QString packageName);
    bool installApk(QString apkPath);
    bool uninstallPackage(QString packageName);
    bool isAppRunning(QString packageName);
    void closeApp(QString packageName);
    void clearData(QString packageName);

    bool isContains(QString key,PropertyType type);
    QPoint getPositionOf(QString key, PropertyType type);
    bool clickOn(QString key, PropertyType type);
    QJsonArray getScreenId();

    bool isNeedToClean();
    double getDataFolderSize();

    //error
    bool isNoInternetAccessGUI();
    bool isAccountDisableGUI();
    bool isIncorrectPassGUI();

    void stop();
    LDPlayerProfile getProfile();
    QString command(QString agruments);
    QString adbCommand(QStringList agruments);
    LDPlayerHelper* getHelper();

    //save variable
    static QPoint usernameInputPos;
    static QPoint passwordInputPos;
    static QPoint loginBtnPos;
    static QPoint continuePos;
    static QPoint loginCodeInput;

    static double value;

public slots:
signals:
    void addLog(QString msg);

private:
    LDPlayerProfile profile;

    LDPlayerHelper *ldhelper = nullptr;

    const QString screenCapturePath = "/sdcard/screencapture.png";

    //interactive
    QImage *avatar = nullptr;
    QList<DefinedScreen*> m_screens;
    QJsonObject m_currentScreen;

    void loadScreen();

    //methods
    void refreshScreenElement();

    QPoint findPosition(QJsonArray screen, QString key, PropertyType type);

    QString propertyToString(PropertyType type);

    static int started;


};

#endif // LDPLAYER_H
