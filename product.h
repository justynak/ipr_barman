#ifndef PRODUCT_H
#define PRODUCT_H
#include <QString>
#include "money.h"

// A product as sold by the bar. Identified by its surrogate id; the name is
// a mutable attribute. `available` is derived from the ingredient ledger and
// the recipe (floor(min(stock / amount))), never stored.
class Product
{
private:
    int _id = 0;
    QString _name;
    Money _price = 0;
    QString _category;
    int _available = 0;

public:
    Product() = default;
    Product(int id, QString name, Money price, QString category, int available)
        : _id(id), _name(name), _price(price), _category(category), _available(available) {}

    int GetId() const {return _id;}
    QString GetName() const {return _name;}
    Money GetPrice() const {return _price;}
    QString GetCategory() const {return _category;}
    int GetAvailable() const {return _available;}

    bool IsValid() const { return _id > 0; }
};

#endif // PRODUCT_H
