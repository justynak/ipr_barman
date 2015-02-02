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

};

#endif // ORDERDETAILS_H
