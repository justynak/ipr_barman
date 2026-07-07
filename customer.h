#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <QString>

struct Customer
{
    int id = 0;         // 0 = not found / invalid
    QString cardNumber;

    bool IsValid() const { return id > 0; }
};

#endif // CUSTOMER_H
