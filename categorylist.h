#ifndef CATEGORYLIST_H
#define CATEGORYLIST_H

#include <QList>
#include "barrepository.h"

class CategoryList
{
private:
    BarRepository *db;
    QList<QString> _categories;

public:
    explicit CategoryList(BarRepository* repository);

    bool GetCategoriesFromDB();

    QList<QString> GetCategories();

};

#endif // CATEGORYLIST_H
