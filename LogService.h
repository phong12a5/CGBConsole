#ifndef LOGSERIVCE_H
#define LOGSERIVCE_H

#include <QObject>
#include <QThread>

class LogService : public QObject
{
    Q_OBJECT
private:
    explicit LogService(QObject *parent = nullptr);

public:
    static LogService* instance();

signals:
    void dologging(QString moduleName,QString funcName, QString msg);

public slots:
    void onDoLogging(QString moduleName,QString funcName, QString msg);

private:
    static LogService* m_instance;

    QThread* m_thread;
};

#endif // LOGSERIVCE_H
