#include "orderdetails.h"

OrderDetails::OrderDetails()
{
    _order = NULL;
    _cost = 0.00;
    _discount = 0.0;
}

OrderDetails::OrderDetails(Order *o)
{
    _order = o;
    _cost = 0.00;
    _discount = 0.0;

    RecalculateCost();

    if(_order->GetCustomerID() != "")
        _discount = LOYAL_CUSTOMER_DISCOUNT;
}

QList<Product> OrderDetails::GetProductList()
{
    if(_order == NULL)
        return QList<Product>();
    return _order->GetProductList();
}

QString OrderDetails::GetOrderNumber()
{
    if(_order == NULL)
        return QString("");
    return _order->GetOrderNumber();
}

void OrderDetails::RecalculateCost()
{
    _cost = 0.00;

    if(_order == NULL)
        return;

    foreach(Product p, _order->GetProductList())
    {
        _cost += p.GetNumber() * p.GetPrice();
    }
}
