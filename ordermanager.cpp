#include "ordermanager.h"

OrderManager::OrderManager(QString bartenderNumber)
{
    db = DatabaseConnector::GetInstance();

    //initialize orderList
    _orderList = new OrderList();
    _orderList->SetOrderList(db->GetOrders(bartenderNumber));

    //initialize selected order
    _selectedOrder = &(_orderList->getOrder(0));

    _pManager = new ProductManager();
}

OrderManager::~OrderManager()
{
    delete _orderList;
    delete _pManager;
}

bool OrderManager::CreateOrder()
{
    return true;
}

bool OrderManager::DeleteOrder()
{
    return true;
}

bool OrderManager::AddProduct()
{
    return true;
}

bool OrderManager::ChangeProductNumber()
{
    return true;
}

bool OrderManager::DeleteProduct()
{
       return true;
}

bool OrderManager::ScanCustomer()
{
      return true;
}

bool OrderManager::PrintBill()
{
     return true;
}

bool OrderManager::RefreshData()
{
    return true;
}

QList<QString> OrderManager::GetOrders()
{
    QList<Order> list =  _orderList->GetOrderList();

    QList<QString> nameList;

    foreach(Order o, list)
        nameList.append(o.GetOrderNumber());

    return nameList;
}

QList<Product> OrderManager::GetProducts()
{
    QList<Product> list;
    return list;
}

