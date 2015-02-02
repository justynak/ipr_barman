#ifndef CATEGORYLIST_H
#define CATEGORYLIST_H

#include <QList>
#include "databaseconnector.h"
//#include "product.h"

class CategoryList
{
private:
    DatabaseConnector *db;
    QList<QString> _categories;
    //QList<Product> _productsInCategory;


public:
    CategoryList();
    ~CategoryList();

    bool GetCategoriesFromDB();
    bool GetProductsFromDB();

    QList<QString> GetCategories();

};

#endif // CATEGORYLIST_H
