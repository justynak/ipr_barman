#ifndef ORDERDETAILS_H
#define ORDERDETAILS_H
#include "order.h"

class OrderDetails
{

private:
    Order* _order;
    double _cost;
    double _discount;

public:
    OrderDetails();
    OrderDetails(Order* o);
    ~OrderDetails();

    void SetDiscount(double discount){_discount = discount;}
    double GetDiscount(){return _discount;}
    QList<Product>* GetProductList(){return _order->GetProductList();}
    QString GetOrderNumber(){return _order->GetOrderNumber();}
    double GetCost(){return _cost;}

    void RecalculateCost();


};

#endif // ORDERDETAILS_H
