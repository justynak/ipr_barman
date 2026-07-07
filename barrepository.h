#ifndef BARREPOSITORY_H
#define BARREPOSITORY_H

#include <QList>
#include <QString>

#include "product.h"
#include "order.h"

// Data-access seam for the application. The production implementation is
// MySqlBarRepository; tests use FakeBarRepository (in-memory).
class BarRepository
{
public:
    virtual ~BarRepository() {}

    // bartenders
    virtual bool BartenderExists(QString cardNumber) = 0;
    virtual QString GetBartenderName(QString cardNumber) = 0;
    virtual QString GetBartenderSurname(QString cardNumber) = 0;
    virtual QList<QString> GetBartenderCardNumbers() = 0;

    // products
    virtual QList<QString> GetCategories() = 0;
    virtual QList<Product> GetProductsFromCategory(QString category) = 0;

    // customers
    virtual bool CustomerExists(QString cardNumber) = 0;
    virtual QList<QString> GetCustomerCardNumbers() = 0;
    virtual bool SetCustomerIDinOrder(QString cardNumber, QString orderNumber) = 0;

    // orders
    virtual QList<Order> GetOrders(QString bartenderNumber) = 0;
    virtual QList<Product> GetProductsFromBill(QString billNumber) = 0;
    virtual bool CreateOrder(QString bartenderNumber, Order* newOrder) = 0;
    virtual bool RemoveOrder(QString billNumber) = 0;
    virtual bool CloseOrder(QString billNumber) = 0;
    virtual bool AddProductToOrder(QString billNumber, Product p) = 0;
    virtual bool RemoveProductFromOrder(QString billNumber, Product p) = 0;
    virtual bool ChangeProductNumber(QString billNumber, Product p) = 0;
};

#endif // BARREPOSITORY_H
