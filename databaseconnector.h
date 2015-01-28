#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <QtSql>
#include <QList>
#include <QStringList>

class DatabaseConnector
{
private:
    DatabaseConnector() { db = QSqlDatabase::addDatabase("QMYSQL"); }
    DatabaseConnector(const DatabaseConnector &);
    DatabaseConnector& operator=(const DatabaseConnector&);
    ~DatabaseConnector() {}

    QSqlDatabase db;

public:

    static DatabaseConnector* GetInstance()
    {
        static DatabaseConnector instance;
        return &instance;
    }

    void connect();


};

#endif // DATABASECONNECTOR_H


