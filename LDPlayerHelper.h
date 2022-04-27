#ifndef LDPLAYERHELPER_H
#define LDPLAYERHELPER_H

#include <QObject>
#include <QMessageBox>
#include <QProcess>
#include <QMutex>

#include "ConfigHelper.h"

struct LDPlayerProfile{
    int id = -1;
    QString name = "";
    QString address = "";
    uint topHWND = 0;
    uint bindHWND = 0;
    bool isStarted = false;
    int pid = 0;
    int vboxpid = 0;

};

class LDPlayerHelper : public QObject
{
    Q_OBJECT
public:
    explicit LDPlayerHelper(QObject *parent = nullptr);
    ~LDPlayerHelper();
    static void initWorkspace();
    static QString workspace;
    bool cloneLDPlayer(int numClone);
    bool copyFrom(QString name, int fromIndex);
    void cleanData(int dncsId);
    bool createLDPlayer(int numLd);
    bool deleteLDPlayer(int dnsId);
    void refreshPath();
    void refreshServer();
    double dataSizeOf(int dncsId);
    void start(int index);
    static void killAllReferencesProcess(bool isInStarted = true);
    static void killProcess();
    void closeAllLDPLayer();
    QString getAdbAdress(int dncsId);
    QList<LDPlayerProfile> getListProfile();
    QList<int> getListDnsId();
    QList<LDPlayerProfile> getRunningProfile();
    QList<LDPlayerProfile> getNotRunningProfile();

    int getRunningCount();
    LDPlayerProfile getProfileByDnsId(int dnsId);
    LDPlayerProfile getProfileByName(QString name);
    bool isRunning(int dncsId);

    bool ldConsoleCommand(QStringList agruments,bool waitForFinish = false);
    void adbShellCommand(QString address,QStringList agruments,bool waitForFinish = false);




    QString ldConsoleCommand2(QStringList agruments);
    QString adbShellCommand2(QString address,QStringList agruments, int timeOut = 60000);
    QByteArray adbShellCommand3(QString address,QStringList agruments);
private:
    static const QString dnconsole;
    static const QString ldconsole;
    static const QString adbshell;
    bool Runcmd(QString file,QStringList agruments,bool waitForFinish = false);
    QString Runcmd2(QString file,QStringList agruments, int timeOut = 60000);//ms
    QByteArray Runcmd3(QString file,QStringList agruments);


    QString ldPlayerPath;
    bool validLdPath();

signals:
    void addLog(QString msg);
};

#endif // LDPLAYERHELPER_H
