#ifndef AUTOFARMERAPI_H
#define AUTOFARMERAPI_H

#include <QObject>
#include <QDateTime>
#include <QMutex>

#include "asyncnetworkmanager.h"
#include "backuprestoremanager.h"

struct Action{
    QString action = "";
    QString date_time = "";
    QString id = "";
    QString link_service = "";
    int number = 0;
    int number_success = 0;
    int number_rest = 0;
    QString post_id = "";
    QString service_code_viplike = "";
    QString service_code = "";
    QString status = "";
    QString time_post = "";
    QString uid = "";
    QString url_service = "";
    QString username = "";

    QString comment_id = "";
    QString content = "";
    QString content_2 = "";
    QString content_id = "";
    QString create_date = "";

    QString fanpage_id = "";
};

class AutoFarmerAPI : public QObject
{
    Q_OBJECT
public:
    static AutoFarmerAPI *instance();
    QJsonObject getClone();
    bool hasClone();

    Action getVipLikeAction();
    Action getBuffLikeAction();
    Action getBuffCommentAction();
    Action getLikePageAction();
    Action getFollowAction();

    void updateAction(Action action, bool isDone = true);
signals:

private:
    QMutex mutex;
    static AutoFarmerAPI *m_instance;
    explicit AutoFarmerAPI(QObject *parent = nullptr);

    QList<QJsonObject> m_accounts;
    AsyncNetworkManager *m_network{nullptr};

    //for temp
    void updateCloneSource();
    void loadClones();
};

#endif // AUTOFARMERAPI_H
