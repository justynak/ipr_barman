#include "categorylist.h"

CategoryList::CategoryList(BarRepository* repository)
    : db(repository)
{
    GetCategoriesFromDB();
}

bool CategoryList::GetCategoriesFromDB()
{
    _categories = db->GetCategories();
    return true;
}

QList<QString> CategoryList::GetCategories()
{
    return _categories;
}
