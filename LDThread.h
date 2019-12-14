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
    explicit LDThread(QObject *parent = nullptr, LDIntance* ldInstance = nullptr);
    ~LDThread();

private:
    LDIntance* m_ldInstance;
    LDRunner* m_Worker;
    QThread *m_workerThread;

signals:
    void operate();
    void missionCompleted(LDThread*);

public slots:
    void finishedATask();
};

#endif // LDTHREAD_H
