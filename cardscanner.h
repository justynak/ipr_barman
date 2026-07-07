#ifndef CARDSCANNER_H
#define CARDSCANNER_H

#include <QString>

// Hardware seam for card readers. LoginScanner and CustomerScanner are
// simulators that pick a random known card; tests script the result.
class CardScanner
{
public:
    virtual ~CardScanner() = default;

    // Returns the scanned card number, or "" when nothing could be scanned.
    virtual QString ScanCard() = 0;
};

#endif // CARDSCANNER_H
