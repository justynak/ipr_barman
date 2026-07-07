#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H
#include "productmanager.h"
#include "orderdetails.h"
#include "cardscanner.h"
#include "order.h"
#include "barrepository.h"
#include "orderlist.h"

class OrderManager
{

private:
    ProductManager* _pManager;
    OrderDetails* _oDetails;
    OrderList* _orderList;
    Order* _selectedOrder;
    BarRepository* db;
    CardScanner* _customerScanner;

public:
    OrderManager(BarRepository* repository, CardScanner* customerScanner, QString bartenderNumber);
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

    QList<QString> GetOrders();
    QList<Product> GetProducts();
    QString GetSelectedOrderNumber(){return _selectedOrder != NULL ? _selectedOrder->GetOrderNumber() : QString("");}
    Order* GetSelectedOrder(){return _selectedOrder;}
    ProductManager* GetProductManager(){return _pManager;}

    QString GetCustomerID(){return _selectedOrder != NULL ? _selectedOrder->GetCustomerID() : QString("");}
    OrderDetails* GetOrderDetails(){return _oDetails;}
    double GetCost(){return _oDetails->GetCost();}
};

#endif // ORDERMANAGER_H
