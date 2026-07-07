#ifndef ORDERLIST_H
#define ORDERLIST_H
#include <QList>
#include "order.h"
#include "barrepository.h"

class OrderList
{
public:
    OrderList(BarRepository* repository, QString bartenderNumber);
    void AddOrder(QString bartenderNumber);
    void RemoveOrder(Order o);
    ~OrderList();
    QList<Order> GetOrderList() {return _list;}
    void SetOrderList(QList<Order>list){ _list.clear(); _list = list;}

    Order operator[](int i);
    Order* GetOrderByName(QString billName);
    Order& GetOrder(int i){return _list[i];}
    Order* GetLast(){return &(_list.last());}

private:
    QList<Order> _list;
    BarRepository *db;

};

#endif // ORDERLIST_H
