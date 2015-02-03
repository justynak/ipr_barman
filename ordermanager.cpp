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
    if(_orderList != NULL) delete _orderList;
    delete _pManager;
}

bool OrderManager::SetSelectedOrder(QString name)
{
    Order* o = _orderList->GetOrderByName(name);

    _selectedOrder = o;
    QList<Product>* p = db->GetProductsFromBill(name);
    _selectedOrder->SetProductList(p);

    return true;
}

bool OrderManager::CreateOrder(QString bartenderNumber)
{
    _orderList->AddOrder(bartenderNumber);
    _selectedOrder = _orderList->GetLast();
    return true;
}

bool OrderManager::DeleteOrder()
{
    return true;
}

bool OrderManager::AddProduct()
{
    Product* p = _pManager->GetSelectedProduct();
    QString name = _selectedOrder->GetOrderNumber();

    if(name != "")
    {
        db->AddProductToOrder(name ,*p);
        _selectedOrder->AddProduct(*p);
    }

    //add to list
    return true;
}

bool OrderManager::ChangeProductNumber(Product *p, int newNumber)
{
    _selectedOrder->ChangeProductNumber(*p, newNumber);
    p->SetNumber(newNumber);
    QString billNumber = _selectedOrder->GetOrderNumber();
    db->ChangeProductNumber(billNumber, *p);

    return true;
}

bool OrderManager::DeleteProduct(Product *p)
{
    QString billNumber = _selectedOrder->GetOrderNumber();
    db->RemoveProductFromOrder(billNumber, *p);

    _selectedOrder->RemoveProduct(*p);
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

