#include "productmanager.h"

ProductManager::ProductManager(BarRepository* repository)
{
    db = repository;
    _categoryList = new CategoryList(db);
}

ProductManager::~ProductManager()
{
    delete _categoryList;
}

void ProductManager::SetProducts(QString category)
{
    _availableProducts = db->GetProductsFromCategory(category);
    if(!_availableProducts.isEmpty())
        _productSelected = _availableProducts.first();
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
    for(int i = 0; i < _availableProducts.size(); ++i)
    {
        if(_availableProducts[i].GetName() == name)
            return &(_availableProducts[i]);
    }

    return NULL;
}
