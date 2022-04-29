#ifndef FACEBOOKACTION_H
#define FACEBOOKACTION_H

#include <QObject>
#include "LDPlayer.h"

class FacebookAction : public QObject
{
    Q_OBJECT
public:
    explicit FacebookAction(LDPlayer *player);

    enum ActionState{

    };

    ActionState doLogin();
    ActionState doBackup();
    ActionState doRestore();

signals:

private:
    LDPlayer *player{nullptr};

};

#endif // FACEBOOKACTION_H
