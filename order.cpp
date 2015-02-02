#include "order.h"

Order::Order()
{

}

Order::Order(QString orderNumber)
{
    _orderNumber = orderNumber;
}

Order::~Order()
{
    _productList.clear();
}

void Order::SetProductList(QList<Product> list)
{
    _productList.clear();
    _productList = list;
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
    uint occurence = _productList.indexOf(p);
    if(occurence >=0) _productList[occurence].setNumber(number);
}

