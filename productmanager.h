#ifndef PRODUCTMANAGER_H
#define PRODUCTMANAGER_H
#include "categorylist.h"
#include "product.h"
#include "barrepository.h"

class ProductManager
{

private:
    CategoryList* _categoryList;
    Product _productSelected;
    QList<Product> _availableProducts;
    BarRepository *db;

public:
    explicit ProductManager(BarRepository* repository);
    ~ProductManager();

    void SetProducts(QString category);
    void SetSelectedProduct(Product* p){ if(p != NULL) _productSelected = *p; }

    QList<QString> GetCategoryList();
    QList<Product> GetAvailableProducts(QString category);
    Product* GetSelectedProduct(){return &_productSelected;}
    Product* GetProductByName(QString name);

};

#endif // PRODUCTMANAGER_H
