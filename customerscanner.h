#ifndef CUSTOMERSCANNER_H
#define CUSTOMERSCANNER_H
#include "cardscanner.h"
#include "barrepository.h"

// Simulator: "scans" the card of a random known loyal customer.
class CustomerScanner : public CardScanner
{

private:
    BarRepository* db;

public:
    explicit CustomerScanner(BarRepository* repository);
    ~CustomerScanner();

    QString ScanCard();
};

#endif // CUSTOMERSCANNER_H
