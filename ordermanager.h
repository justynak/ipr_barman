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

    bool SetSelectedOrder(Order* o);
    bool SetSelectedOrder(QString name);
    bool SetSelectedProduct(Product* p){_pManager->SetSelectedProduct(p); return true;}

    bool CreateOrder(QString bartenderNumber);
    bool DeleteOrder();
    bool AddProduct();
    bool ChangeProductNumber(Product* p, int newNumber);

    bool DeleteProduct(Product *p);
    bool ScanCustomer();

    bool CloseOrder();

    bool RefreshData();

    QList<QString> GetOrders();
    QList<Product> GetProducts();
    QString GetSelectedOrderNumber(){return _selectedOrder->GetOrderNumber();}
    Order* GetSelectedOrder(){return _selectedOrder;}
    ProductManager* GetProductManager(){return _pManager;}

    QString GetCustomerID(){return _selectedOrder->GetCustomerID();}
    OrderDetails* GetOrderDetails(){return _oDetails;}
};

#endif // ORDERMANAGER_H
