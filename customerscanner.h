#ifndef CUSTOMERSCANNER_H
#define CUSTOMERSCANNER_H
#include "databaseconnector.h"

class CustomerScanner
{

private:
    DatabaseConnector* db;
    QString _id;

public:
    CustomerScanner();
    bool ScanCustomerID();
    QString GetCustomerID(){return _id;}
    ~CustomerScanner();
};

#endif // CUSTOMERSCANNER_H
