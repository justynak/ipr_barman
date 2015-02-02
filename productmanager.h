#ifndef PRODUCTMANAGER_H
#define PRODUCTMANAGER_H
#include "categorylist.h"
#include "product.h"
#include "order.h"

class ProductManager
{

private:
    CategoryList _categoryList;
    Product _productSelected;


public:
    ProductManager();
    ~ProductManager();
    AddProduct(Order o);
    ChangeProductNumber(Order o, Product p, uint newNumber);
};

#endif // PRODUCTMANAGER_H
