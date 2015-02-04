#ifndef CATEGORYLIST_H
#define CATEGORYLIST_H

#include <QList>
#include "databaseconnector.h"

class CategoryList
{
private:
    DatabaseConnector *db;
    QList<QString> _categories;

public:
    CategoryList();
    ~CategoryList();

    bool GetCategoriesFromDB();
    bool GetProductsFromDB();

    QList<QString> GetCategories();

};

#endif // CATEGORYLIST_H
