#include <DbManager.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <AppDefines.h>

DbManager::DbManager(const QString &path)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    if (!m_db.open()) {
        LOGD("Error: connection with database fail");
    } else {
        LOGD("Database: connection ok");
    }
}

DbManager::~DbManager()
{
    if (m_db.isOpen())
    {
        m_db.close();
    }
}

bool DbManager::isOpen() const
{
    return m_db.isOpen();
}

bool DbManager::addObject()
{
    LOGD("");
    bool success = false;
    QSqlQuery queryAdd;
    queryAdd.prepare("INSERT INTO uid_policy(logging, desired_name, username, policy, until, command, uid, desired_uid, package_name, name, notification) values(:logging, :desired_name, :username, :policy, :until, :command, :uid, :desired_uid, :package_name, :name, :notification)");
    queryAdd.bindValue(":logging",0);
    queryAdd.bindValue(":desired_name",QVariant());
    queryAdd.bindValue(":username",QVariant());
    queryAdd.bindValue(":policy","allow");
    queryAdd.bindValue(":until",0);
    queryAdd.bindValue(":command","");
    queryAdd.bindValue(":uid",10034);
    queryAdd.bindValue(":desired_uid",0);
    queryAdd.bindValue(":package_name","xyz.autofarmer.app");
    queryAdd.bindValue(":name","AutoFarmer");
    queryAdd.bindValue(":notification",0);

    if(queryAdd.exec())  {
        success = true;
    } else {
        LOGD("add object failed: " + QString(queryAdd.lastError().text()));
    }
    return success;
}


void DbManager::printAllPersons() const
{
    LOGD("");
    QSqlQuery query("SELECT * FROM uid_policy");
    int idName = query.record().indexOf("name");
    while (query.next()) {
        QString name = query.value(idName).toString();
        LOGD("===" + name);
    }
}

bool DbManager::objectExists() const
{
    bool exists = false;
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT name FROM uid_policy WHERE name = (:name)");
    checkQuery.bindValue(":name", "AutoFarmer");

    if (checkQuery.exec()) {
        if (checkQuery.next()) {
            exists = true;
        }
    } else {
        LOGD(QString("person exists failed: ") + checkQuery.lastError().text());
    }
    LOGD(exists);
    return exists;
}

bool DbManager::updateRow()
{
    LOGD("");
    bool success = false;
    QSqlQuery query;
    query.prepare("UPDATE uid_policy SET logging=:logging, desired_name=:desired_name, username=:username, policy=:policy, until=:until, command=:command, uid=:uid, desired_uid=:desired_uid, package_name=:package_name, notification=:notification WHERE name=:name");
    query.bindValue(":logging",0);
    query.bindValue(":desired_name",QVariant());
    query.bindValue(":username",QVariant());
    query.bindValue(":policy","allow");
    query.bindValue(":until",0);
    query.bindValue(":command","");
    query.bindValue(":uid",10034);
    query.bindValue(":desired_uid",0);
    query.bindValue(":package_name","xyz.autofarmer.app");
    query.bindValue(":notification",0);
    query.bindValue(":name","AutoFarmer");

    if (query.exec()) {
        success = true;
    } else {
        LOGD("Update failed: " + query.lastError().text());
    }
    return success;
}

void DbManager::close()
{
    if(m_db.isOpen()) m_db.close();
}
