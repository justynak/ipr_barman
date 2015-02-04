#include "orderdetails.h"

OrderDetails::OrderDetails()
{

}

OrderDetails::OrderDetails(Order *o)
{
    _order = o;
    _cost = 0.00;
    _discount = 0.0;

    QList<Product>list =  *(_order->GetProductList());

    foreach(Product p, list)
    {
        _cost += p.GetNumber() * p.GetPrice();
    }
}

OrderDetails::~OrderDetails()
{

}

