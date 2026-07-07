#ifndef MYSQLBARREPOSITORY_H
#define MYSQLBARREPOSITORY_H

#include <QtSql>
#include <QList>
#include <QStringList>

#include "barrepository.h"

struct BarDatabaseConfig
{
    QString hostName;
    QString databaseName;
    QString userName;
    QString password;
    QString barName;
};

class MySqlBarRepository : public BarRepository
{
private:
    BarDatabaseConfig _config;
    QSqlDatabase db;

public:
    explicit MySqlBarRepository(const BarDatabaseConfig& config);
    ~MySqlBarRepository();

    // Opens the connection; on failure the error is available via LastError().
    bool Open();
    QSqlError LastError() const { return db.lastError(); }

    Employee FindEmployeeByCard(QString cardNumber);
    QList<QString> GetEmployeeCardNumbers();
    int OpenShift(int employeeId, QDateTime openedAt);
    bool CloseShift(int shiftId, QDateTime closedAt);

    QList<QString> GetCategories();
    QList<Product> GetProductsFromCategory(QString category);

    Customer FindCustomerByCard(QString cardNumber);
    QList<QString> GetCustomerCardNumbers();

    QString GetBarCurrency();

    bool FinalizeOrder(const DraftOrder& draft, int shiftId,
                       QDate businessDay, QDateTime createdAt);
};

#endif // MYSQLBARREPOSITORY_H
