#include "ordermanager.h"


OrderManager::OrderManager(BarRepository* repository, CardScanner* customerScanner, QString bartenderNumber)
{
    db = repository;
    _customerScanner = customerScanner;

    //initialize orderList
    _orderList = new OrderList(db, bartenderNumber);

    //no order selected until the user picks one (the list may be empty)
    _selectedOrder = NULL;

    _oDetails = new OrderDetails();

    _pManager = new ProductManager(db);
}

OrderManager::~OrderManager()
{
    if(_orderList != NULL) delete _orderList;
    delete _oDetails;
    delete _pManager;
}

bool OrderManager::SetSelectedOrder(Order *o)
{
    if(o == NULL) return false;

    if(_oDetails != NULL) delete _oDetails;
    _oDetails  = new OrderDetails(o);

    _selectedOrder = o;
    return true;
}

bool OrderManager::SetSelectedOrder(QString name)
{
    Order* o = _orderList->GetOrderByName(name);
    if(o == NULL) return false;

    o->SetProductList(db->GetProductsFromBill(name));

    if(_oDetails != NULL) delete _oDetails;
    _oDetails  = new OrderDetails(o);

    _selectedOrder = o;

    return true;
}

bool OrderManager::CreateOrder(QString bartenderNumber)
{
    _orderList->AddOrder(bartenderNumber);
    return SetSelectedOrder(_orderList->GetLast());
}

bool OrderManager::DeleteOrder()
{
    if(_selectedOrder == NULL) return false;

    QString billNumber = _selectedOrder->GetOrderNumber();

    foreach(Product p, _selectedOrder->GetProductList())
    {
        db->RemoveProductFromOrder(billNumber, p);
        _selectedOrder->RemoveProduct(p);
    }

    db->RemoveOrder(billNumber);

    return true;
}

bool OrderManager::AddProduct()
{
    if(_selectedOrder == NULL) return false;

    Product* p = _pManager->GetSelectedProduct();
    QString name = _selectedOrder->GetOrderNumber();

    if(name != "")
    {
        db->AddProductToOrder(name ,*p);
        _selectedOrder->AddProduct(*p);
    }

    //add to list
    _oDetails->RecalculateCost();
    return true;
}

bool OrderManager::ChangeProductNumber(Product *p, int newNumber)
{
    if(_selectedOrder == NULL) return false;

    _selectedOrder->ChangeProductNumber(*p, newNumber);
    p->SetNumber(newNumber);
    QString billNumber = _selectedOrder->GetOrderNumber();
    db->ChangeProductNumber(billNumber, *p);
    _oDetails->RecalculateCost();

    return true;
}

bool OrderManager::DeleteProduct(Product *p)
{
    if(_selectedOrder == NULL) return false;

    QString billNumber = _selectedOrder->GetOrderNumber();
    db->RemoveProductFromOrder(billNumber, *p);

    _selectedOrder->RemoveProduct(*p);
    _oDetails->RecalculateCost();

    return true;
}

bool OrderManager::ScanCustomer()
{
    if(_selectedOrder == NULL) return false;

    QString customerNumber = _customerScanner->ScanCard();

    // The discount is only for cards of known loyal customers.
    if(customerNumber == "" || !db->CustomerExists(customerNumber))
        return false;

    _oDetails->SetDiscount(LOYAL_CUSTOMER_DISCOUNT);

    _selectedOrder->SetCustomerID(customerNumber);
    db->SetCustomerIDinOrder(customerNumber, _selectedOrder->GetOrderNumber());

    return true;
}

bool OrderManager::CloseOrder()
{
    if(_selectedOrder == NULL) return false;

    QString orderNumber = _selectedOrder->GetOrderNumber();
    db->CloseOrder(orderNumber);
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
    if(_selectedOrder == NULL)
        return QList<Product>();
    return _selectedOrder->GetProductList();
}
