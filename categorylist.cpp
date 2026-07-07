#include "categorylist.h"

CategoryList::CategoryList(BarRepository* repository)
{
    db = repository;
    this->GetCategoriesFromDB();
}

CategoryList::~CategoryList()
{

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
