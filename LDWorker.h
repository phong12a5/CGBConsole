#ifndef LDWORKER_H
#define LDWORKER_H

#include <QObject>
#include <QTimer>

class LDWorker : public QObject
{
    Q_OBJECT
public:
    explicit LDWorker(QString name, QObject *parent = nullptr);
    ~LDWorker();

public slots:
    void onStarted();
    void onCheckRunningApp();
signals:
    void start();

private:
    QString m_name;
    QTimer* m_appChecker;
    bool app_running = false;
};

#endif // LDWORKER_H
