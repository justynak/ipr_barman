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

    void SetCardNumber(QString number) {_scannedNumber = number;}
    QString GetCardNumber(){return _scannedNumber;}
    bool ScanCard();

    bool CheckLogin(QString login);
};

#endif // LOGINSCANNER_H
