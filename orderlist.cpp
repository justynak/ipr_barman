#include "orderlist.h"

OrderList::OrderList(QString bartenderNumber)
{
    db->DatabaseConnector::GetInstance();
    _list = db->GetOrders(bartenderNumber);
}

void OrderList::AddOrder(Order p)
{
    _list.append(p);
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

Order OrderList::GetOrderByName(QString billName)
{
    Order o(billName);

    int i = _list.lastIndexOf(o);
    return _list[i];
}

