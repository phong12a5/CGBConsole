#ifndef LDWORKER_H
#define LDWORKER_H

#include <QObject>

class LDWorker : public QObject
{
    Q_OBJECT
public:
    explicit LDWorker(QString name, QObject *parent = nullptr);
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
    QString m_name;
    bool app_running = false;
    bool m_started;
};

#endif // LDWORKER_H
