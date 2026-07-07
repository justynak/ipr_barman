#include "bartender.h"

Bartender::Bartender(BarRepository* repository, CardScanner* customerScanner, QString pesel) : _pesel(pesel)
{
    db = repository;

    _name = db->GetBartenderName(_pesel);
    _surname = db->GetBartenderSurname(_pesel);

    //init order manager
    _oManager = new OrderManager(db, customerScanner, _pesel);
}

Bartender::~Bartender()
{
    if(_oManager != NULL) delete _oManager;
}

void Bartender::SetName()
{
    _name = db->GetBartenderName(_pesel);
}

void Bartender::SetSurname()
{
    _surname = db->GetBartenderSurname(_pesel);
}

bool Bartender::AddProduct()
{
    _oManager->AddProduct();
    return true;
}

bool Bartender::RemoveProduct(Product p)
{
    _oManager->DeleteProduct(&p);
    return true;
}

bool Bartender::ChangeProductNumber(Product p, int newNumber)
{
    _oManager->ChangeProductNumber(&p, newNumber);
    return true;
}

bool Bartender::RemoveOrder()
{
    _oManager->DeleteOrder();
    return true;
}

bool Bartender::AddOrder()
{
    _oManager->CreateOrder(_pesel);
    return true;
}

bool Bartender::CloseOrder()
{
    _oManager->CloseOrder();
    return true;
}

bool Bartender::ScanCustomer()
{
    _oManager->ScanCustomer();
    return true;
}

bool Bartender::SetOrder(QString billNumber)
{
    _oManager->SetSelectedOrder(billNumber);
    return true;
}

QList<QString> Bartender::GetOrders()
{
    return _oManager->GetOrders();
}

QList<Product> Bartender::GetProductsFromOrder()
{
    return _oManager->GetProducts();
}

