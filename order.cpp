#include "order.h"

Order::Order()
{
    _orderNumber = "";
    _customerID = "";
}

Order::Order(QString orderNumber)
{
    _orderNumber = orderNumber;
    _customerID = "";
}

void Order::SetProductList(QList<Product> list)
{
    _productList = list;
}

QList<Product> Order::GetProductList()
{
    return _productList;
}

void Order::AddProduct(Product p)
{
    _productList.append(p);
}

void Order::RemoveProduct(Product p)
{
    _productList.removeOne(p);
}

void Order::ChangeProductNumber(Product p, int number)
{
    int occurence = _productList.indexOf(p);
    if(occurence >= 0) _productList[occurence].SetNumber(number);
}

bool Order::operator ==(Order o1)
{
    if(o1.GetOrderNumber() == this->_orderNumber)
        return true;
    else
        return false;
}
