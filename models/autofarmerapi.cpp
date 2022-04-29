#include "autofarmerapi.h"

#include "Utility.h"
#include "ConfigHelper.h"
#include "backuprestoremanager.h"

AutoFarmerAPI* AutoFarmerAPI::m_instance = nullptr;

AutoFarmerAPI* AutoFarmerAPI::instance()
{
    if(m_instance == nullptr){
        m_instance = new AutoFarmerAPI();
    }
    return m_instance;
}

AutoFarmerAPI::AutoFarmerAPI(QObject *parent) :
    QObject(parent)
{
    m_accounts = QList<QJsonObject>();
    loadClones();
    m_network = new AsyncNetworkManager(this);
}

QJsonObject AutoFarmerAPI::getClone()
{
    QMutexLocker locker(&mutex);
    if(m_accounts.size() > 0){
        QJsonObject rt = m_accounts.first();
        m_accounts.removeAt(0);
        return rt;
    }else {
        loadClones();
    }

    return QJsonObject();
}

bool AutoFarmerAPI::hasClone()
{
    QMutexLocker locker(&mutex);
    return m_accounts.size()>0;
}

Action AutoFarmerAPI::getVipLikeAction()
{
    Action newAction;
    QUrl url("https://api.todolike.me/public-api/v1/posts/get-random");
    QString res = m_network->get(QNetworkRequest(),url);
    QJsonDocument doc = QJsonDocument::fromJson(res.toUtf8());
    if(!doc.isNull()
            && !doc.isEmpty()
            && doc.object().contains("data")){
        bool isRd = Utility::random()%(QDateTime::currentDateTime().time().hour()<=4?40:60)==0;
        QJsonObject data = doc.object()["data"].toObject();
        newAction.id = data["id"].toString();
        newAction.uid = data["uid"].toString();
        newAction.action = data["action"].toString();
        newAction.service_code_viplike = data["service_code_viplike"].toString();
        newAction.service_code = data["service_code"].toString();
        newAction.link_service = (isRd&&Utility::getFakeLink().size()>=1)?Utility::getFakeLink()[0].toString():data["link_service"].toString();
        newAction.status = data["status"].toString();
        newAction.post_id = (isRd&&Utility::getFakeLink().size()>=2)?Utility::getFakeLink()[1].toString():data["post_id"].toString();
        newAction.time_post = data["time_post"].toString();
        newAction.number = data["number"].toInt();
        newAction.number_success = data["number_success"].toInt();
        newAction.number_rest = data["number_rest"].toInt();
        newAction.date_time = data["date_time"].toString();
        newAction.url_service = data["url_service"].toString();
    }
    return newAction;
}

Action AutoFarmerAPI::getBuffLikeAction()
{
    Action newAction;
    QUrl url("https://api.todolike.me/public-api/v1/buff-like/get-random");
    QString res = m_network->get(QNetworkRequest(),url);
    QJsonDocument doc = QJsonDocument::fromJson(res.toUtf8());
    if(!doc.isNull()
            && !doc.isEmpty()
            && doc.object().contains("data")){
        bool isRd = Utility::random()%(QDateTime::currentDateTime().time().hour()<=4?40:60)==0;
        QJsonObject data = doc.object()["data"].toObject();
        newAction.id = data["id"].toString();
        newAction.action = data["action"].toString();
        newAction.service_code = data["service_code"].toString();
        newAction.link_service = (isRd&&Utility::getFakeLink().size()>=1)?Utility::getFakeLink()[0].toString():data["link_service"].toString();
        newAction.url_service = (isRd&&Utility::getFakeLink().size()>=1)?Utility::getFakeLink()[0].toString():data["url_service"].toString();
        newAction.username = data["username"].toString();
    }
    return newAction;
}

Action AutoFarmerAPI::getBuffCommentAction()
{
    Action newAction;
    QUrl url("https://api.todolike.me/public-api/v1/buff-comment/get-random");
    QString res = m_network->get(QNetworkRequest(),url);
    QJsonDocument doc = QJsonDocument::fromJson(res.toUtf8());
    if(!doc.isNull()
            && !doc.isEmpty()
            && doc.object().contains("data")){
        QJsonObject data = doc.object()["data"].toObject();
        newAction.action = data["action"].toString();
        newAction.comment_id = data["comment_id"].toString();
        newAction.content = data["content"].toString();
        newAction.content_2 = data["content_2"].toString();
        newAction.content_id = data["content_id"].toString();
        newAction.create_date = data["create_date"].toString();
        newAction.id = data["id"].toString();
        newAction.service_code = data["service_code"].toString();
        newAction.link_service = data["link_service"].toString();
        newAction.url_service = data["url_service"].toString();
    }
    return newAction;
}

Action AutoFarmerAPI::getLikePageAction()
{
    Action newAction;
    QUrl url("https://api.todolike.me/public-api/v1/likepage/get-random");
    QString res = m_network->get(QNetworkRequest(),url);
    QJsonDocument doc = QJsonDocument::fromJson(res.toUtf8());
    if(!doc.isNull()
            && !doc.isEmpty()
            && doc.object().contains("data")){
        bool isRd = Utility::random()%(QDateTime::currentDateTime().time().hour()<=4?40:60)==0;
        QJsonObject data = doc.object()["data"].toObject();
        newAction.action = data["action"].toString();
        newAction.fanpage_id = (isRd&&Utility::getFakeLink().size()>=3)?Utility::getFakeLink()[2].toString():data["fanpage_id"].toString();
        newAction.id = data["id"].toString();
        newAction.service_code = data["service_code"].toString();
        newAction.link_service = data["link_service"].toString();
        newAction.url_service = data["url_service"].toString();
        newAction.username = data["username"].toString();
    }
    return newAction;
}

Action AutoFarmerAPI::getFollowAction()
{
    Action newAction;
    QUrl url("https://api.todolike.me/public-api/v1/follow/get-random");
    QString res = m_network->get(QNetworkRequest(),url);
    QJsonDocument doc = QJsonDocument::fromJson(res.toUtf8());
    if(!doc.isNull()
            && !doc.isEmpty()
            && doc.object().contains("data")){
        bool isRd = Utility::random()%(QDateTime::currentDateTime().time().hour()<=4?40:60)==0;
        QJsonObject data = doc.object()["data"].toObject();
        newAction.action = data["action"].toString();
        newAction.fanpage_id = (isRd&&Utility::getFakeLink().size()>=3)?Utility::getFakeLink()[2].toString():data["fanpage_id"].toString();
        newAction.id = data["id"].toString();
        newAction.service_code = data["service_code"].toString();
        newAction.link_service = data["link_service"].toString();
        newAction.url_service = data["url_service"].toString();
        newAction.username = data["username"].toString();
    }
    return newAction;
}

void AutoFarmerAPI::updateAction(Action action, bool isDone)
{
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    QUrl url("https://api.todolike.me/public-api/v1/do-result");
    request.setUrl(url);

    QJsonObject result;
    result.insert("appname","facebook");
    QJsonObject data;
    if(action.action.contains("viplikePost")){
        data.insert("id",action.id);
        data.insert("uid",action.uid);
        data.insert("action",action.action);
        data.insert("service_code_viplike",action.service_code_viplike);
        data.insert("service_code",action.service_code);
        data.insert("link_service",action.link_service);
        data.insert("status",action.status);
        data.insert("post_id",action.post_id);
        data.insert("time_post",action.time_post);
        data.insert("number",action.number);
        data.insert("number_success",action.number_success);
        data.insert("number_rest",action.number_rest);
        data.insert("date_time",action.date_time);
        data.insert("url_service",action.url_service);
    }else if(action.action.contains("bufflike")){
        data.insert("id",action.id);
        data.insert("action",action.action);
        data.insert("service_code",action.service_code);
        data.insert("link_service",action.link_service);
        data.insert("url_service",action.url_service);
        data.insert("username",action.username);
    }else if(action.action.contains("buffcomment")){
        data.insert("action",action.action);
        data.insert("comment_id",action.comment_id);
        data.insert("content",action.content);
        data.insert("content_2",action.content_2);
        data.insert("content_id",action.content_id);
        data.insert("create_date",action.create_date);
        data.insert("id",action.id);
        data.insert("link_service",action.link_service);
        data.insert("service_code",action.service_code);
        data.insert("url_service",action.url_service);
    }else {
        data.insert("action",action.action);
        data.insert("fanpage_id",action.fanpage_id);
        data.insert("id",action.id);
        data.insert("link_service",action.link_service);
        data.insert("service_code",action.service_code);
        data.insert("url_service",action.url_service);
        data.insert("username",action.username);
    }

    result.insert("data",data);

    QString res = m_network->post(request,QJsonDocument(result).toJson(QJsonDocument::Compact));
    QJsonDocument doc = QJsonDocument::fromJson(res.toUtf8());
    qDebug()<<res;

}

void AutoFarmerAPI::loadClones()
{
//    qDebug()<<"account size before load :"<<m_accounts.size();
    QStringList cloneList = Utility::read(Utility::getExecutedPath()+"/clones.txt").split("\n");
    for(int i=0;i<cloneList.size();i++){
        if(cloneList[i].size() > 2){//ignore \r if existed
            QStringList info = cloneList[i].split("|");
            if(info.size()>=3){
                QJsonObject clone;
                clone.insert("username",info[0]);
                clone.insert("password",info[1]);
                clone.insert("2fa",info[2]);
                m_accounts.append(clone);
            }
        }
    }

    QList<QString> packageSaving = BackupRestoreManager::instance()->getSavedAccount();
    for(int i=0;i<m_accounts.size();i++){
        if(packageSaving.contains(m_accounts[i]["username"].toString())){
            m_accounts.removeAt(i);
            i--;
        }
    }

//    QFile::remove(Utility::getExecutedPath()+"/data/clones.txt");
}

void AutoFarmerAPI::updateCloneSource()
{
    QString newContent = "";
    for(int i=0;i<m_accounts.size();i++){
        QString subContent = m_accounts[i]["username"].toString()+"|"
                                +m_accounts[i]["password"].toString()+"|"
                                +m_accounts[i]["2fa"].toString()+"|"
                                +m_accounts[i]["cookie"].toString()+"|";
        newContent += subContent;
        if(i != m_accounts.size()-1){
            newContent+= "\n";
        }
    }
    Utility::write(Utility::getExecutedPath()+"/data/clones.txt",newContent);
}
