#ifndef PRODUCTMANAGER_H
#define PRODUCTMANAGER_H
#include "categorylist.h"
#include "product.h"
#include "order.h"
#include "databaseconnector.h"

class ProductManager
{

private:
    CategoryList* _categoryList;
    Product* _productSelected;
    QList<Product> _availableProducts;
    DatabaseConnector *db;

public:
    ProductManager();
    ~ProductManager();
    //void AddProduct(Order o);
    void SetProducts(QString category);
    void SetSelectedProduct(Product* p){_productSelected = p;}
    void ChangeProductNumber(Order o, Product p, uint newNumber);

    QList<QString> GetCategoryList();
    QList<Product> GetAvailableProducts(QString category);
    Product* GetSelectedpProduct(){return _productSelected;}


};

#endif // PRODUCTMANAGER_H
