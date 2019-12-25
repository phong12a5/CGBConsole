#ifndef DOWNLOADSERVICE_H
#define DOWNLOADSERVICE_H

#include <QObject>
#include <QThread>
#include <AppDefines.h>

class DownloadService : public QObject
{
    Q_OBJECT
public:
    explicit DownloadService(QObject *parent = nullptr);

private:
    QThread* m_thread;

signals:
    void operate();

public slots:
    void run();
};

#endif // DOWNLOADSERVICE_H
