#ifndef SCRIPTEDCARDSCANNER_H
#define SCRIPTEDCARDSCANNER_H

#include "cardscanner.h"

// Test double: always "scans" the card it was told to.
class ScriptedCardScanner : public CardScanner
{
public:
    explicit ScriptedCardScanner(QString card = QString("")) : _card(card) {}

    void SetCard(QString card) { _card = card; }

    QString ScanCard() { return _card; }

private:
    QString _card;
};

#endif // SCRIPTEDCARDSCANNER_H
