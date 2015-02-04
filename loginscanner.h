#ifndef LOGINSCANNER_H
#define LOGINSCANNER_H
#include <QString>
#include "databaseconnector.h"


class LoginScanner
{
private:
    QString _scannedNumber;
    DatabaseConnector *db;

public:
    LoginScanner();
    ~LoginScanner();

    QString GetCardNumber(){return _scannedNumber;}
    bool ScanCard();
};

#endif // LOGINSCANNER_H
