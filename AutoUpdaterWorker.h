#ifndef AUTOUPDATERWORKER_H
#define AUTOUPDATERWORKER_H

#include <QObject>

class AutoUpdaterWorker : public QObject
{
    Q_OBJECT
public:
    explicit AutoUpdaterWorker(QObject *parent = nullptr);

    enum E_FINISHED_CODE: int {
        E_FINISHED_CODE_NO_CHANGED = 0,
        E_FINISHED_CODE_NEW_VERSION,
        E_FINISHED_CODE_UPDATE_FAILURE
    };

signals:
    void updateFinished(int code);

public slots:
    void doWork();
};

#endif // AUTOUPDATERWORKER_H
