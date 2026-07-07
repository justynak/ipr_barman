#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>

struct Employee
{
    int id;             // 0 = not found / invalid
    QString role;       // "bartender" or "manager"
    QString cardNumber;
    QString firstName;
    QString lastName;

    Employee() : id(0) {}

    bool IsValid() const { return id > 0; }
};

#endif // EMPLOYEE_H
