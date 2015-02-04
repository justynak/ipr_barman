#include "bartender.h"

Bartender::Bartender(QString pesel) : _pesel(pesel)
{
    db = DatabaseConnector::GetInstance();

    QString name = db->GetBartenderName(_pesel);
    QString surname = db->GetBartenderSurame(_pesel);

    _name = name;
    _surname = surname;

    //init order manager
    _oManager = new OrderManager(_pesel);
}

Bartender::~Bartender()
{
    delete _oManager;
}

void Bartender::SetName()
{
    _name = db->GetBartenderName(_pesel);
}

void Bartender::SetSurname()
{
    _name = db->GetBartenderSurame(_pesel);
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
    _oManager->PrintBill();
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

