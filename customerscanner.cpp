#include "customerscanner.h"

CustomerScanner::CustomerScanner()
{
    db = DatabaseConnector::GetInstance();
    _id = "";
}

bool CustomerScanner::ScanCustomerID()
{
    _id = db->GetRandomCustomerID();
    return db->CustomerExists(_id);
}


CustomerScanner::~CustomerScanner()
{

}

