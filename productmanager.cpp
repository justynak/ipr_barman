#include "productmanager.h"

ProductManager::ProductManager()
{
    db = DatabaseConnector::GetInstance();
    _categoryList = new CategoryList();
}

ProductManager::~ProductManager()
{
    delete _categoryList;
}

//void ProductManager::AddProduct(Order o)
//{
    //o.AddProduct(*_productSelected);
//}

void ProductManager::SetProducts(QString category)
{
    _availableProducts = db->GetProductsFromCategory(category);
    _productSelected = &(_availableProducts[0]);
}

void ProductManager::ChangeProductNumber(Order o, Product p, uint newNumber)
{
    o.ChangeProductNumber(p, newNumber);
}

QList<QString> ProductManager::GetCategoryList()
{
    return _categoryList->GetCategories();
}

QList<Product> ProductManager::GetAvailableProducts(QString category)
{
    QList<Product> list;
    list = db->GetProductsFromCategory(category);
    return list;
}

