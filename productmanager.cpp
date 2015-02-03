#include "productmanager.h"

ProductManager::ProductManager()
{
    db = DatabaseConnector::GetInstance();
    _categoryList = new CategoryList();
    _productSelected = new Product();
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
    _availableProducts = db->GetProductsFromCategory(category);
    return _availableProducts;
}

Product *ProductManager::GetProductByName(QString name)
{
    int occurence = -1;
    bool exists = false;

    foreach(Product p, _availableProducts)
    {
        ++occurence;
        if(p.GetName() == name)
        {
            exists = true;
            break;
        }
    }

    if(exists) return &(_availableProducts[occurence]);
    else return NULL;
}

