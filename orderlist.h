#ifndef ORDERLIST_H
#define ORDERLIST_H
#include <QList>
#include "order.h"


class OrderList
{
public:
    OrderList();
    void AddOrder(Order o);
    void RemoveOrder(Order o);
    ~OrderList();
    QList<Order> GetOrderList() {return _list;}
    void SetOrderList(QList<Order>list){ _list.clear(); _list = list;}

    Order operator[](int i);
    Order getOrder(int i){return _list[i];}

private:
    QList<Order> _list;

};

#endif // ORDERLIST_H
