#ifndef ORDER_H
#define ORDER_H
#include <QDate>
#include "product.h"


class Order
{
private:
    QList<Product>* _productList;
    QString _orderNumber;
    QDate _date;

public:
    Order();
    Order(QString orderNumber);
    ~Order();

    void SetProductList(QList<Product>* list);

    QList<Product>* GetProductList();

    void AddProduct(Product p);
    void RemoveProduct(Product p);
    void ChangeProductNumber(Product p, int number);


    QString GetOrderNumber(){return _orderNumber;}

    bool operator ==(Order o1);

};

#endif // ORDER_H
