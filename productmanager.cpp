#include "productmanager.h"

ProductManager::ProductManager(BarRepository* repository)
    : _categoryList(new CategoryList(repository)),
      db(repository)
{
}

ProductManager::~ProductManager()
{
    delete _categoryList;
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

    return nullptr;
}
