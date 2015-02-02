#ifndef LOGINSCANNER_H
#define LOGINSCANNER_H
#include <QString>

class LoginScanner
{
private:
    QString _scannedNumber;


public:
    LoginScanner();
    ~LoginScanner();

    void SetCardNumber(QString number) {_scannedNumber = number;}
    QString GetCardNumber(){return _scannedNumber;}
};

#endif // LOGINSCANNER_H
