#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H
#include "productmanager.h"
#include "orderdetails.h"
#include "customerscanner.h"
#include "order.h"
#include "databaseconnector.h"
#include "orderlist.h"

class OrderManager
{

private:
    ProductManager* _pManager;
    OrderDetails* _oDetails;
    OrderList* _orderList;
    Order* _selectedOrder;
    DatabaseConnector* db;

public:
    OrderManager(){}
    OrderManager(QString bartenderNumber);
    ~OrderManager();

    bool SetSelectedOrder(Order* o) {_selectedOrder = o; return true;}
    bool SetSelectedOrder(QString name);

    bool CreateOrder(QString bartenderNumber);
    bool DeleteOrder();
    bool AddProduct();
    bool ChangeProductNumber();

    bool DeleteProduct();
    bool ScanCustomer();

    bool PrintBill();

    bool RefreshData();

    QList<QString> GetOrders();
    QList<Product> GetProducts();
    QString GetSelectedOrder(){return _selectedOrder->GetOrderNumber();}
};

#endif // ORDERMANAGER_H
