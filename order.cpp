#include "order.h"

Order::Order()
{

}

Order::Order(QString orderNumber)
{
    _orderNumber = orderNumber;
    _productList = new QList<Product>;
}

Order::~Order()
{
    //_productList.clear();
}

void Order::SetProductList(QList<Product> *list)
{
    _productList->clear();
    _productList = list;
}

QList<Product>* Order::GetProductList()
{
    if(_productList != NULL)
        return this->_productList;

    else
        return NULL;
}

void Order::AddProduct(Product p)
{
    _productList->append(p);
}

void Order::RemoveProduct(Product p)
{
    _productList->removeOne(p);
}

void Order::ChangeProductNumber(Product p, int number)
{
    uint occurence = _productList->indexOf(p);
    if(occurence >=0) (*_productList)[occurence].SetNumber(number);
}

bool Order::operator ==(Order o1)
{
    if(o1.GetOrderNumber() == this->_orderNumber)
        return true;
    else
        return false;
}

