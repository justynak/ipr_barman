#ifndef ORDER_H
#define ORDER_H
#include <QDate>
#include "product.h"

class Order
{
private:
    QList<Product> _productList;
    QString _productNumber;
    QDate _date;

public:
    Order();
    ~Order();
};

#endif // ORDER_H
