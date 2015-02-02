#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <QtSql>
#include <QList>
#include <QStringList>

#include "product.h"
#include "order.h"
#include "local.h"

#define LOCAL_NAME  "Boboli_8_Warszawa"

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

    QList<Product> GetProductsFromCategory(QString category);
    QList<QString> GetCategories();

    bool BartenderExists(QString number);
    QString GetRandomBartender();

    QString GetBartenderName(QString number);
    QString GetBartenderSurame(QString number);

    QList<Product> GetProductsFromBill(QString billNumber);
    QList<Order> GetOrders(QString bartenderNumber);

    bool RemoveProductFromOrder(QString billNumber, Product p);
    bool AddProductToOrder(QString billNumber, Product p);
    bool ChangeProductNumber(QString billNumber, Product p);
    bool CloseOrder(QString billNumber);
    bool RemoveOrder(QString billNumber);

};

#endif // DATABASECONNECTOR_H


