#include "ordermanager.h"

OrderManager::OrderManager(QString bartenderNumber)
{
    db = DatabaseConnector::GetInstance();

    //initialize orderList
    _orderList = new OrderList(bartenderNumber);

    //initialize selected order
    _selectedOrder = &(_orderList->GetOrder(0));

    _pManager = new ProductManager();
}

OrderManager::~OrderManager()
{
    delete _orderList;
    delete _pManager;
}

bool OrderManager::SetSelectedOrder(QString name)
{
    Order* o = &(_orderList->GetOrderByName(name));

    if(o != NULL)
    {
        _selectedOrder = o;
        QList<Product>* p = db->GetProductsFromBill(name);
        _selectedOrder->SetProductList(p);
        return true;
    }
    else return false;
}

bool OrderManager::CreateOrder(QString bartenderNumber)
{
    _orderList->AddOrder(bartenderNumber);
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
    QList<Product> emptyList;
    QList<Product>* p = (_selectedOrder->GetProductList());
    if(p!= NULL)
        return *p;
    else
        return emptyList;

}

