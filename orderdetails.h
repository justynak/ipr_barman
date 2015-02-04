#ifndef ORDERDETAILS_H
#define ORDERDETAILS_H
#include "order.h"

class OrderDetails
{

private:
    Order* _order;
    double _cost;

public:
    OrderDetails();
    OrderDetails(Order* o);
    ~OrderDetails();
    QList<Product>* GetProductList(){return _order->GetProductList();}
    QString GetOrderNumber(){return _order->GetOrderNumber();}
    double GetCost(){return _cost;}


};

#endif // ORDERDETAILS_H
