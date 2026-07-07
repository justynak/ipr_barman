#include "customerscanner.h"
#include <QRandomGenerator>

CustomerScanner::CustomerScanner(BarRepository* repository)
{
    db = repository;
}

CustomerScanner::~CustomerScanner()
{

}

QString CustomerScanner::ScanCard()
{
    QList<QString> cards = db->GetCustomerCardNumbers();

    if(cards.isEmpty())
        return QString("");

    return cards[QRandomGenerator::global()->bounded(cards.size())];
}
