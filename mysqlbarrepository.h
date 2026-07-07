#ifndef MYSQLBARREPOSITORY_H
#define MYSQLBARREPOSITORY_H

#include <QtSql>
#include <QList>
#include <QStringList>

#include "barrepository.h"
#include "product.h"
#include "order.h"

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

    bool BartenderExists(QString cardNumber);
    QString GetBartenderName(QString cardNumber);
    QString GetBartenderSurname(QString cardNumber);
    QList<QString> GetBartenderCardNumbers();

    QList<QString> GetCategories();
    QList<Product> GetProductsFromCategory(QString category);

    bool CustomerExists(QString cardNumber);
    QList<QString> GetCustomerCardNumbers();
    bool SetCustomerIDinOrder(QString cardNumber, QString orderNumber);

    QList<Order> GetOrders(QString bartenderNumber);
    QList<Product> GetProductsFromBill(QString billNumber);
    bool CreateOrder(QString bartenderNumber, Order* newOrder);
    bool RemoveOrder(QString billNumber);
    bool CloseOrder(QString billNumber);
    bool AddProductToOrder(QString billNumber, Product p);
    bool RemoveProductFromOrder(QString billNumber, Product p);
    bool ChangeProductNumber(QString billNumber, Product p);
};

#endif // MYSQLBARREPOSITORY_H
