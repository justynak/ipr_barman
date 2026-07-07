#include "bartender.h"

Bartender::Bartender(BarRepository* repository, CardScanner* customerScanner,
                     Employee employee, int shiftId)
{
    db = repository;
    _employee = employee;
    _shiftId = shiftId;
    _currency = db->GetBarCurrency();

    //init order manager
    _oManager = new OrderManager(db, customerScanner);
}

Bartender::~Bartender()
{
    if(_oManager != NULL) delete _oManager;
}
