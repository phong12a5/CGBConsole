#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>

/**
 * \class DbManager
 *
 * \brief SQL Database Manager class
 *
 * DbManager sets up the connection with SQL database
 * and performs some basics queries. The class requires
 * existing SQL database. You can create it with sqlite:
 * 1. $ sqlite3 people.db
 * 2. sqilte> CREATE TABLE people(ids integer primary key, name text);
 * 3. sqlite> .quit
 */
class DbManager
{
public:
    DbManager(const QString& path);

    ~DbManager();

    bool isOpen() const;

    bool addObject();

    bool objectExists() const;

    void printAllPersons() const;

    bool updateRow();

    void close();

private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
