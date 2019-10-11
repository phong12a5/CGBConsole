#ifndef LDRUNNER_H
#define LDRUNNER_H

#include <QObject>
#include "AppDefines.h"
#include <QTimer>

class LDRunner : public QObject
{
    Q_OBJECT
public:
    explicit LDRunner(QString instanceName);
    ~LDRunner();

private:
    QString m_instanceName;
    bool m_setIsLDFile;
    QTimer *m_checkConnectionTimer;
    QTimer *m_checkEndScriptTimer;
    QTimer *m_checkRunAppTimer;
    QTimer *m_checkRunningDevice;

public:

signals:
    void finished();

public slots:
    void run();
    void quitRunner();
    void onCheckConnection();
    void onCheckEnscript();
    void onCheckRunApp();
    void onCheckRunningDevice();
};

#endif // LDRUNNER_H
