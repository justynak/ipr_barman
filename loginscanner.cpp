#include "loginscanner.h"
#include <QRandomGenerator>

LoginScanner::LoginScanner(BarRepository* repository)
{
   db = repository;
}

LoginScanner::~LoginScanner()
{

}

QString LoginScanner::ScanCard()
{
   QList<QString> cards = db->GetBartenderCardNumbers();

   if(cards.isEmpty())
       return QString("");

   return cards[QRandomGenerator::global()->bounded(cards.size())];
}
