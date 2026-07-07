#ifndef BARTENDER_H
#define BARTENDER_H
#include <QString>
#include "ordermanager.h"
#include "barrepository.h"
#include "cardscanner.h"
#include "employee.h"

// Facade over the managers for the logged-in employee's shift.
class Bartender
{

private:
    Employee _employee;
    int _shiftId;

    OrderManager* _oManager;
    BarRepository *db;

public:
    Bartender(BarRepository* repository, CardScanner* customerScanner,
              Employee employee, int shiftId);
    ~Bartender();

    QString GetName(){return _employee.firstName;}
    QString GetSurname(){return _employee.lastName;}
    QString GetRole(){return _employee.role;}
    int GetShiftId(){return _shiftId;}

    bool AddProduct(){return _oManager->AddProduct();}
    bool ChangeProductQuantity(int productId, int quantity){return _oManager->ChangeProductQuantity(productId, quantity);}
    bool RemoveProduct(int productId){return _oManager->DeleteProduct(productId);}

    bool AddOrder(){return _oManager->CreateOrder();}
    bool RemoveOrder(){return _oManager->DeleteOrder();}
    bool FinalizeOrder(){return _oManager->FinalizeOrder(_shiftId);}

    bool ScanCustomer(){return _oManager->ScanCustomer();}

    bool SetOrder(QString label){return _oManager->SetSelectedOrder(label);}
    QList<QString> GetOrders(){return _oManager->GetOrders();}
    QList<OrderLine> GetOrderLines(){return _oManager->GetSelectedOrder().GetLines();}
    DraftOrder GetSelectedDraft(){return _oManager->GetSelectedOrder();}

    QString GetSelectedOrderNumber(){return _oManager->GetSelectedOrderNumber();}
    ProductManager* GetProductManager(){return _oManager->GetProductManager();}

    QString GetSelectedOrderCustomerID(){return _oManager->GetCustomerID();}
    Money GetSelectedOrderCost(){return _oManager->GetCost();}

};

#endif // BARTENDER_H
