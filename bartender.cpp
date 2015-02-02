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

bool Bartender::AddProduct(Product p)
{
    _oManager->AddProduct();
    return true;
}

bool Bartender::RemoveProduct(Product p)
{
    _oManager->DeleteProduct();
    return true;
}

bool Bartender::ChangeProductNumber(Product p)
{
    _oManager->ChangeProductNumber();
    return true;
}

bool Bartender::RemoveOrder(QString billNumber)
{
    _oManager->DeleteOrder();
    return true;
}

bool Bartender::AddOrder()
{
    _oManager->CreateOrder();
    return true;
}

QList<QString> Bartender::GetOrders()
{
    return _oManager->GetOrders();
}

QList<Product> Bartender::GetProductsFromOrder(QString billNumber)
{
    QList<Product> list;
    return list;
}

