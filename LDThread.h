#ifndef LDTHREAD_H
#define LDTHREAD_H

#include <QObject>
#include "AppDefines.h"
#include "LDIntance.h"
#include "LDRunner.h"
#include <QThread>

class LDThread : public QObject
{
    Q_OBJECT
public:
    explicit LDThread(QObject *parent = nullptr, QString ldInstanceName = "");
    ~LDThread();

private:
    void setLDInstance();

private:
    LDIntance* m_ldInstance;
    LDRunner* m_Worker;
    QThread* m_workerThread;
    QString m_ldInstanceName;

signals:
    void operate();
    void quitThread();
    void missionCompleted(LDThread*);

public slots:
    void finishedATask();
};

#endif // LDTHREAD_H
