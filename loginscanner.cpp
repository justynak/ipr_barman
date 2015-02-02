#include "loginscanner.h"

LoginScanner::LoginScanner()
{
   db = DatabaseConnector::GetInstance();

}

LoginScanner::~LoginScanner()
{

}

bool LoginScanner::ScanCard()
{
   _scannedNumber = db->GetRandomBartender();

   return db->BartenderExists(_scannedNumber);
}

