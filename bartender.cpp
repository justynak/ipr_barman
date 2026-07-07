#include "bartender.h"

Bartender::Bartender(BarRepository* repository, CardScanner* customerScanner,
                     Employee employee, int shiftId)
    : _employee(employee),
      _shiftId(shiftId),
      _currency(repository->GetBarCurrency()),
      _oManager(new OrderManager(repository, customerScanner)),
      db(repository)
{
}

Bartender::~Bartender()
{
    delete _oManager;
}
