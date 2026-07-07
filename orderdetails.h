#ifndef ORDERDETAILS_H
#define ORDERDETAILS_H
#include "order.h"

// Discount rate for loyal customers — the single definition, used by both
// OrderDetails and OrderManager.
const double LOYAL_CUSTOMER_DISCOUNT = 0.1;

class OrderDetails
{

private:
    Order* _order;
    double _cost;
    double _discount;

public:
    OrderDetails();
    OrderDetails(Order* o);

    void SetDiscount(double discount){_discount = discount;}
    double GetDiscount(){return _discount;}
    QList<Product> GetProductList();
    QString GetOrderNumber();
    double GetCost(){return _cost;}
    double GetDiscountedCost(){return _cost * (1.0 - _discount);}

    void RecalculateCost();


};

#endif // ORDERDETAILS_H
