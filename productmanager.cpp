#include "productmanager.h"

ProductManager::ProductManager()
{

}

ProductManager::~ProductManager()
{

}

ProductManager::AddProduct(Order o)
{
    o.AddProduct(_productSelected);
}

ProductManager::ChangeProductNumber(Order o, Product p, uint newNumber)
{
    o.ChangeProductNumber(p, newNumber);
}

