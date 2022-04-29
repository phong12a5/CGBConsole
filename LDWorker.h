#ifndef LDWORKER_H
#define LDWORKER_H

#include <QObject>

#include "LDPlayer.h"
#include "models/backuprestorepackage.h"

class LDWorker : public QObject
{
    Q_OBJECT
public:
    explicit LDWorker(int ldIndex, QObject *parent = nullptr);
    ~LDWorker();

    enum State{
        UNKNOWN_STATE = -1,
        PREPARE_FAILED = 1,
        PREPARE_SUCCESS = 2,
        LOAD_VIETNAMESE_FAILED = 3,
        CONTINUE = 4,
        GET_CLONE = 5,
        FINISH = 6
    };

public slots:
    void start();
    void stop();
    void run();

signals:
    void post();

private:
    void postDelay(int milSec);

private:
    int m_ldIndex;
    bool app_running = false;
    bool m_started;
    bool isGetElementOk = false;
    bool openLD;

    LDPlayer *player{nullptr};
    BackupRestorePackage *m_package{nullptr};

    State doLogin(FacebookAccount &acc);
    State preparePackage();
};

#endif // LDWORKER_H
