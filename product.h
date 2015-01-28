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

    bool setName(QString name) { _name = name; return true;}
    bool setNumber(uint number) {_number = number; return true;}
    bool setPrice(double price) {_price = price; return true;}
};

#endif // PRODUCT_H
