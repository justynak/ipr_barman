#ifndef LOGINSCANNER_H
#define LOGINSCANNER_H
#include <QString>
#include "cardscanner.h"
#include "barrepository.h"


// Simulator: "scans" the card of a random known employee.
class LoginScanner : public CardScanner
{
private:
    BarRepository *db;

public:
    explicit LoginScanner(BarRepository* repository);

    QString ScanCard() override;
};

#endif // LOGINSCANNER_H
