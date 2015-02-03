#ifndef ORDERLIST_H
#define ORDERLIST_H
#include <QList>
#include "order.h"
#include "databaseconnector.h"

class OrderList
{
public:
    OrderList(QString bartenderNumber);
    void AddOrder(QString bartenderNumber);
    void RemoveOrder(Order o);
    ~OrderList();
    QList<Order> GetOrderList() {return _list;}
    void SetOrderList(QList<Order>list){ _list.clear(); _list = list;}

    Order operator[](int i);
    Order GetOrderByName(QString billName);
    Order GetOrder(int i){return _list[i];}

private:
    QList<Order> _list;
    DatabaseConnector *db;

};

#endif // ORDERLIST_H
