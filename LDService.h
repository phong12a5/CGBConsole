#ifndef LDSERVICE_H
#define LDSERVICE_H

#include <QObject>
#include <QTimer>
#include <QMap>
#include <QThread>

class LDWorker;

class LDService : public QObject
{
    Q_OBJECT
public:
    enum E_SERVICE_STATUS: int {
        E_SERVICE_STATUS_INITIALIZING = 0,
        E_SERVICE_STATUS_RUNNING,
        E_SERVICE_STATUS_STOPPED,
        E_SERVICE_STATUS_UNKNOWN
    };

    explicit LDService(int serviceId, QObject *parent = nullptr);
    ~LDService();

    E_SERVICE_STATUS serviceStatus() const;
    void setServiceStatus(E_SERVICE_STATUS);

    void disposeService();
    void startService();

public slots:
    void onThreadStarted();
    void onThreadFinished();

signals:
    void statusChanged(int);

private:
    static LDService* m_instance;

    int m_serviceId;
    E_SERVICE_STATUS m_serviceStatus;
    QThread* m_ldThread;
    LDWorker* m_ldWorker;
};

#endif // LDSERVICE_H
