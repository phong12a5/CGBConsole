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
    void onStarted();

signals:
    void start();

private:
    QString m_name;
};

#endif // LDWORKER_H
