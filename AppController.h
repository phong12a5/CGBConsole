#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <QTimer>
#include "AppDefines.h"
#include "AppModel.h"
#include "PerformanceReader.h"
#include <LDService.h>

class AppController : public QObject
{
    Q_OBJECT
private:
    explicit AppController(QObject *parent = nullptr);

public:
    static AppController* instance();
    void initAppController();

    void startLDPlayers();
    void stopLDPlayers();

private:
    LDService* createLDService();

public slots:
    void onLDServiceUpdate(int serviceId);
    void onCheckLDServices();

private:
    static AppController* m_instance;

    QMap<int, LDService*> m_ldServiceMap;
    QTimer* m_ldObserver;

public slots:
};

#endif // APPCONTROLLER_H
