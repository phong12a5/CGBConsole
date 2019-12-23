#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <QTimer>
#include "AppDefines.h"
#include "AppModel.h"
#include "LDThread.h"
#include "PerformanceReader.h"

class AppController : public QObject
{
    Q_OBJECT
private:
    explicit AppController(QObject *parent = nullptr);

public:
    static AppController* instance();
    void initAppController();

    void startMultiTask();
    void stopMultiTask();

private:
    static AppController* m_instance;
    QList<QObject*> m_deviceQueue;
    QList<LDThread* > m_ldThreadList;
    QTimer m_updateLDThreadList;

public slots:
    void aMissionCompleted(LDThread* threadAdd = nullptr);
    void onUpdateLDThreadList();
};

#endif // APPCONTROLLER_H
