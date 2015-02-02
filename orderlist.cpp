#include "orderlist.h"

OrderList::OrderList()
{

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

