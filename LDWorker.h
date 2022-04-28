#ifndef LDWORKER_H
#define LDWORKER_H

#include <QObject>
#include "LDPlayer.h"

class LDWorker : public QObject
{
    Q_OBJECT
public:
    explicit LDWorker(int ldIndex, QObject *parent = nullptr);
    ~LDWorker();

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
};

#endif // LDWORKER_H
