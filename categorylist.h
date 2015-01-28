#ifndef CATEGORYLIST_H
#define CATEGORYLIST_H
#include "product.h"
#include <QList>
#include "databaseconnector.h"

class CategoryList
{
private:
    QList<QString> _categories;
    QList<Product> _productsInCategory;
    DatabaseConnector* _db;


public:
    CategoryList();
    ~CategoryList();

    bool GetCategoriesFromDB();
    bool GetProductsFromDB();

    QList<QString> GetCategories();

};

#endif // CATEGORYLIST_H
