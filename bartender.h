#ifndef BARTENDER_H
#define BARTENDER_H
#include <QString>
#include "ordermanager.h"
class Bartender
{

private:
    QString _pesel;
    OrderManager _oManager;
public:
    Bartender();
    ~Bartender();
};

#endif // BARTENDER_H
