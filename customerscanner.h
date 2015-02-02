#ifndef CUSTOMERSCANNER_H
#define CUSTOMERSCANNER_H
#include "databaseconnector.h"

class CustomerScanner
{

private:
    DatabaseConnector* database;

public:
    CustomerScanner();
    ~CustomerScanner();
};

#endif // CUSTOMERSCANNER_H
