#include "databaseconnector.h"
#include <QMessageBox>
#include <QObject>

void DatabaseConnector::connect()
{
    db.setHostName("localhost");
    db.setDatabaseName("mydb");
    db.setUserName("bar");
    db.setPassword("kelner2015");

    if(db.open() == false)
    {
       QSqlError err = db.lastError();
       QMessageBox::information(0, QObject::tr("Błąd"), err.text());
    }
}


QList<Product> DatabaseConnector::GetProductsFromCategory(QString category)
{
    QList<Product> list;
    QSqlQuery q;

    QString command = QString("select a.nazwa, cena_aktualna, min(s.ilosc) from d_bar_artykul "
                              "join m_artykul as a on a.nazwa = d_bar_artykul.d_numer_artykulu"
                              "join m_artykul_skladnik as a_s on a.nazwa = a_s.m_Artykul_nazwa "
                              "join m_skladnik as s on a_s.m_Skladnik_nazwa = s.nazwa  "
                              "where kategoria = \"%1\" and d_nazwa_baru = '%2' "
                              "group by a.nazwa ").arg(category);

    q.exec(command);

    while(q.next())
    {
        Product p;
        QString name = q.value(0).toString();
        double price = q.value(1).toDouble();
        int count = q.value(2).toInt();
        p.SetName(name);
        p.SetNumber(count);
        p.SetPrice(price);
        list.append(p);
    }
    return list;

}

QList<QString> DatabaseConnector::GetCategories()
{
    QList<QString> list;
    QSqlQuery q;
    QString command = QObject::tr("SELECT DISTINCT kategoria FROM "
                                  "d_bar_artykul as b join m_artykul as a "
                                  "on b.d_numer_artykulu = a.nazwa "
                                  "WHERE b.d_numer_baru = '%1' order by kategoria").arg(LOCAL_NAME);
    q.exec(command);

    while(q.next())
        list.append(q.value(0).toString());
    return list;
}

bool DatabaseConnector::BartenderExists(QString number)
{
    this->connect();

    QSqlQuery q;
    QString command = QObject::tr("SELECT * FROM  d_barman WHERE pesel = '%1' ").arg(number);
    q.exec(command);

    q.first();
    if(q.value(0).isNull()) return false;
    return true;
}

QString DatabaseConnector::GetRandomBartender()
{
    this->connect();

    QSqlQuery q;
    QString command = QObject::tr("SELECT pesel FROM  d_barman");
    q.exec(command);

    QList<QString> list;

    while(q.next())
        list.append(q.value(0).toString());

    uint max = list.size();
    uint i = qrand() % max;

    return list[i];
}

QString DatabaseConnector::GetBartenderName(QString number)
{
    this->connect();

    QSqlQuery q;
    QString command = QObject::tr("SELECT imie FROM  d_barman WHERE pesel = '%1'").arg(number);
    q.exec(command);

    q.first();

    QString name = q.value(0).toString();
    return name;
}

QString DatabaseConnector::GetBartenderSurame(QString number)
{
    this->connect();

    QSqlQuery q;
    QString command = QObject::tr("SELECT nazwisko FROM  d_barman WHERE pesel = '%1'").arg(number);
    q.exec(command);

    q.first();

    QString surname = q.value(0).toString();
    return surname;
}

QList<Product> DatabaseConnector::GetProductsFromBill(QString billNumber)
{
    this->connect();

    QSqlQuery q;
    QString command = QObject::tr(" FROM `d_zamowienie_artykul` WHERE pesel = '%1'").arg(billNumber);
    q.exec(command);

    QList<Product> list;

    while(q.next())
    {   //nazwa, l szt, cena
        Product p;
        p.SetName(q.value(0).toString());
        p.SetNumber(q.value(1).toInt());
        p.SetPrice(q.value(2).toDouble());
        list.append(p);
    }

    return list;
}

QList<Order> DatabaseConnector::GetOrders(QString bartenderNumber)
{
    this->connect();

    QSqlQuery q;
    QString command = QObject::tr("SELECT nr_zamowienia FROM  d_zamowienie WHERE d_barman_pesel = '%1'").arg(bartenderNumber);
    q.exec(command);

    QList<Order> list;

    while(q.next())
    {   //nazwa, l szt, cena
        QString orderNumber = q.value(0).toString();
        Order o(orderNumber);
        o.SetProductList(GetProductsFromBill(orderNumber));
        list.append(o);
    }

    return list;
}



bool DatabaseConnector::RemoveProductFromOrder(QString billNumber, Product p)
{
    this->connect();

    QSqlQuery q;
    QString command = QObject::tr("delete from`d_zamowienie_artykul` "
                                  "where `d_numer_zamowienia` = '%1' "
                                  "and `d_nazwa_artykulu` = '%2' "
                                  "and `ilosc` = '%3'").arg(billNumber).arg(p.GetName()).arg(p.GetNumber());
    q.exec(command);
    q.finish();

    command = QObject::tr("update `m_skladnik` set `ilosc` = ilosc + %1"
                 "where `nazwa` in "
                 "(select `m_Skladnik_nazwa` from `m_artykul_skladnik` "
                 "where `m_Artykul_nazwa` = \"%2\" )").arg(p.GetNumber()).arg(p.GetName());

    return q.exec(command);
}

bool DatabaseConnector::AddProductToOrder(QString billNumber, Product p)
{
    this->connect();

    QSqlQuery q;
    QString command = QObject::tr("insert into `d_zamowienie_artykul` (`d_numer_zamowienia`, `d_nazwa_artykulu`, `ilosc`)"
                                  "values ( '%1', '%2' ,'%3')").arg(billNumber).arg(p.GetName()).arg(p.GetNumber());
    q.exec(command);
    q.finish();

    command = QObject::tr("update `m_skladnik` set `ilosc` = `ilosc` - %1 "
                          "where `nazwa` in "
                          "(select `m_Skladnik_nazwa` from `m_artykul_skladnik` "
                          "where `m_Artykul_nazwa` = \"%2\" )").arg(p.GetNumber()).arg(p.GetName());
    return q.exec(command);
}


bool DatabaseConnector::ChangeProductNumber(QString billNumber, Product p)
{
    this->connect();
    int available;

    QSqlQuery q;
    QString command_n = QString("update `d_zamowienie_artykul` "
                                "set ilosc = %3 "
                                "where d_nazwa_artykulu = '%1' "
                                "and d_numer_zamowienia = %2 ").arg(p.GetName()).arg(billNumber).arg(p.GetNumber());
    q.exec(command_n);
    q.first();
    available = q.value(0).toInt();

    QString command = QObject::tr("update `m_skladnik` set `ilosc` = %1 "
                     "where `nazwa` in "
                     "(select `m_Skladnik_nazwa` from `m_artykul_skladnik` "
                     "where `m_Artykul_nazwa` = \"%2\" )").arg(p.GetNumber()).arg(p.GetPrice());

    return q.exec(command);
}



bool DatabaseConnector::CloseOrder(QString billNumber)
{
    this->connect();
    QSqlQuery q;
    QString command = QObject::tr("UPDATE d_zamowienie SET zamkniety= '1' "
                                  "WHERE nr_zamowienia = %1").arg(billNumber);
    return q.exec(command);
}

bool DatabaseConnector::RemoveOrder(QString billNumber)
{
    this->connect();

    QSqlQuery query;
    QString command = QObject::tr("delete from `d_zamowienie_artykul` where d_numer_zamowienia =%1").arg(billNumber);
    query.exec(command);
    query.finish();

    command = QObject::tr("delete from `d_zamowienie` where nr_zamowienia =%1").arg(billNumber);
    return query.exec(command);
}



