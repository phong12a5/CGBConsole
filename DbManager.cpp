#include <dbmanager.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <AppDefines.h>

DbManager::DbManager(const QString &path)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    if (!m_db.open()) {
        LOG << "Error: connection with database fail";
    } else {
        LOG << "Database: connection ok";
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
    LOG;
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
        LOG << "add object failed: " << queryAdd.lastError();
    }
    return success;
}


void DbManager::printAllPersons() const
{
    LOG;
    QSqlQuery query("SELECT * FROM uid_policy");
    int idName = query.record().indexOf("name");
    while (query.next()) {
        QString name = query.value(idName).toString();
        LOG << "===" << name;
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
        LOG << "person exists failed: " << checkQuery.lastError();
    }
    LOG << exists;
    return exists;
}

bool DbManager::updateRow()
{
    LOG;
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
        LOG << "Update failed: " << query.lastError();
    }
    return success;
}

void DbManager::close()
{
    if(m_db.isOpen()) m_db.close();
}
