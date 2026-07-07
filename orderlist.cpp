#include "orderlist.h"

OrderList::OrderList(QString bartenderNumber)
{
    db = DatabaseConnector::GetInstance();
    _list = db->GetOrders(bartenderNumber);
}

void OrderList::AddOrder(QString bartenderNumber)
{
    Order o;
    db->CreateOrder(bartenderNumber, &o);

    _list.append(o);
}

void OrderList::RemoveOrder(Order p)
{
    _list.removeOne(p);
}

OrderList::~OrderList()
{

}

Order OrderList::operator[](int i)
{
    return _list[i];
}

Order* OrderList::GetOrderByName(QString billName)
{
    Order o(billName);

    int i = _list.lastIndexOf(o);
    if(i < 0)
        return NULL;
    return &(_list[i]);
}
