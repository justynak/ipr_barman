#ifndef PRODUCT_H
#define PRODUCT_H
#include <QString>

class Product
{
private:
    QString _name;
    uint _number;
    double _price;

public:
    Product();
    Product(QString name, uint number, double price) : _name(name), _number(number), _price(price) {}
    ~Product();

    bool SetName(QString name) { _name = name; return true;}
    bool SetNumber(uint number) {_number = number; return true;}
    bool SetPrice(double price) {_price = price; return true;}

    bool operator ==(Product p1);

    QString GetName(){return _name;}
    uint GetNumber(){return _number;}
    double GetPrice(){return _price;}
};

#endif // PRODUCT_H
