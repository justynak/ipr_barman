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

    bool AddProduct(Product p);
    bool RemoveProduct(Product p);
    bool ChangeProductNumber(Product p);

    bool RemoveOrder(QString billNumber);
    bool AddOrder();

    QList<QString> GetOrders();
    QList<Product> GetProductsFromOrder(QString billNumber);
};

#endif // BARTENDER_H
