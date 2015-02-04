#ifndef BARTENDER_H
#define BARTENDER_H
#include <QString>
#include "ordermanager.h"
#include "databaseconnector.h"

class Bartender
{

private:
    QString _pesel;
    QString _name;
    QString _surname;

    OrderManager* _oManager;
    DatabaseConnector *db;

public:
    Bartender(QString pesel);
    Bartender(){db = DatabaseConnector::GetInstance();}
    ~Bartender();

    void SetName();
    void SetSurname();
    void SetPesel(QString pesel){ _pesel = pesel;}

    QString GetName(){return _name;}
    QString GetPesel(){return _pesel;}
    QString GetSurname(){return _surname;}

    bool AddProduct();
    bool RemoveProduct(Product p);
    bool ChangeProductNumber(Product p, int newNumber);

    bool RemoveOrder();
    bool AddOrder();
    bool CloseOrder();

    bool ScanCustomer();

    bool SetSelectedProduct(Product* p) {_oManager->SetSelectedProduct(p); return true;}

    bool SetOrder(QString billNumber);
    QList<QString> GetOrders();
    QList<Product> GetProductsFromOrder();

    QString GetSelectedOrderNumber(){return _oManager->GetSelectedOrderNumber();}
    Order* GetSelectedOrder(){return _oManager->GetSelectedOrder();}
    ProductManager* GetProductManager(){return _oManager->GetProductManager();}

    OrderDetails* GetOrderDetails(){return _oManager->GetOrderDetails();}
    QString GetSelectedOrderCustomerID(){return _oManager->GetCustomerID();}
    double GetSelectedOrderCost(){return _oManager->GetCost();}

};

#endif // BARTENDER_H
