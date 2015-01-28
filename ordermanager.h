#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H
#include "productmanager.h"
#include "orderdetails.h"
#include "customerscanner.h"
#include "order.h"

class OrderManager
{

private:
    ProductManager _pManager;
    OrderDetails _oDetails;
    //lista rachunków??


public:
    OrderManager();
    ~OrderManager();
};

#endif // ORDERMANAGER_H
