#include "orderdetails.h"

OrderDetails::OrderDetails()
{

}

OrderDetails::OrderDetails(Order *o)
{
    _order = o;

    QList<Product>list =  *(_order->GetProductList());

    foreach(Product p, list)
    {
        _cost += p.GetNumber() * p.GetPrice();
    }
}

OrderDetails::~OrderDetails()
{

}

