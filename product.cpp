#include "product.h"

Product::Product()
{

}

Product::~Product()
{

}

bool Product::operator ==(Product p1)
{
    if(p1.GetName() == this->_name &&
        p1.GetNumber() == this->_number)
        return true;
    else return false;
}

