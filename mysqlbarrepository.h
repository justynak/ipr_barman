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

    // Opens the connection; on failure the error is available via LastError().
    bool Open();
    QSqlError LastError() const { return db.lastError(); }

    Employee FindEmployeeByCard(QString cardNumber) override;
    QList<QString> GetEmployeeCardNumbers() override;
    int OpenShift(int employeeId, QDateTime openedAt) override;
    bool CloseShift(int shiftId, QDateTime closedAt) override;

    QList<QString> GetCategories() override;
    QList<Product> GetProductsFromCategory(QString category) override;

    Customer FindCustomerByCard(QString cardNumber) override;
    QList<QString> GetCustomerCardNumbers() override;

    QString GetBarCurrency() override;

    bool FinalizeOrder(const DraftOrder& draft, int shiftId,
                       QDate businessDay, QDateTime createdAt) override;
};

#endif // MYSQLBARREPOSITORY_H
