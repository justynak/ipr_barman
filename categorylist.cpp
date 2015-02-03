#include "categorylist.h"

CategoryList::CategoryList()
{
    db = DatabaseConnector::GetInstance();
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


bool CategoryList::GetProductsFromDB()
{
    return true;
}

QList<QString> CategoryList::GetCategories()
{
    return _categories;
}

