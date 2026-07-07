#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H
#include "productmanager.h"
#include "cardscanner.h"
#include "draftorder.h"
#include "barrepository.h"

// Keeps the drafts being composed at the bar. Every mutation is memory-only;
// the DB is touched exactly once per order, in FinalizeOrder.
class OrderManager
{

private:
    ProductManager* _pManager;
    BarRepository* db;
    CardScanner* _customerScanner;

    QList<DraftOrder> _drafts;
    int _selected;          // index into _drafts, -1 = none
    int _nextDraftNumber;

    DraftOrder* Selected();

public:
    OrderManager(BarRepository* repository, CardScanner* customerScanner);
    ~OrderManager();

    QList<QString> GetOrders();
    bool CreateOrder();
    bool SetSelectedOrder(QString label);
    bool DeleteOrder();

    bool AddProduct();
    bool ChangeProductQuantity(int productId, int quantity);
    bool DeleteProduct(int productId);

    bool ScanCustomer();

    // The one write: composes the order, its lines and the ledger movements
    // in a single repository transaction, then drops the draft from memory.
    bool FinalizeOrder(int shiftId);

    bool HasSelectedOrder(){return _selected >= 0;}
    DraftOrder GetSelectedOrder();
    QString GetSelectedOrderNumber();
    QString GetCustomerID();
    Money GetCost();
    ProductManager* GetProductManager(){return _pManager;}
};

#endif // ORDERMANAGER_H
