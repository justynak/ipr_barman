#include "orderdetails.h"
#define DISCOUNT 0.1

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

    if(_order->GetCustomerID() != "" || _order->GetCustomerID() != "")
        _discount = DISCOUNT;
}

OrderDetails::~OrderDetails()
{

}

void OrderDetails::RecalculateCost()
{
    _cost = 0.00;

    QList<Product>list =  *(_order->GetProductList());

    foreach(Product p, list)
    {
        _cost += p.GetNumber() * p.GetPrice();
    }
}

