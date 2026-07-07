#include "databaseconnector.h"
#include <QMessageBox>
#include <QObject>
#include <QDate>


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

    QString command = QString("select p.name, current_price, min(i.quantity) from bar_product "
                              "join product as p on p.name = bar_product.product_name "
                              "join product_ingredient as p_i on p.name = p_i.product_name "
                              "join ingredient as i on p_i.ingredient_name = i.name  "
                              "where category = \"%1\" and bar_id = '%2' "
                              "group by p.name ").arg(category).arg(LOCAL_NAME);

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
    QString command = QObject::tr("SELECT DISTINCT category FROM "
                                  "bar_product as b join product as p "
                                  "on b.product_name = p.name "
                                  "WHERE b.bar_id = '%1' order by category").arg(LOCAL_NAME);
    q.exec(command);

    while(q.next())
        list.append(q.value(0).toString());
    return list;
}

bool DatabaseConnector::BartenderExists(QString number)
{
    QSqlQuery q;
    QString command = QObject::tr("SELECT * FROM  bartender WHERE card_number = '%1' ").arg(number);
    q.exec(command);

    q.first();
    if(q.value(0).isNull()) return false;
    return true;
}

QString DatabaseConnector::GetRandomBartender()
{
    QSqlQuery q;
    QString command = QObject::tr("SELECT card_number FROM  bartender");
    q.exec(command);

    QList<QString> list;

    while(q.next())
        list.append(q.value(0).toString());

    qsrand(QTime::currentTime().msec());
    uint max = list.size();
    uint i = qrand() % max;

    return list[i];
}

QString DatabaseConnector::GetBartenderName(QString number)
{
    QSqlQuery q;
    QString command = QObject::tr("SELECT first_name FROM  bartender WHERE card_number = '%1'").arg(number);
    q.exec(command);

    q.first();

    QString name = q.value(0).toString();
    return name;
}

QString DatabaseConnector::GetBartenderSurame(QString number)
{
    QSqlQuery q;
    QString command = QObject::tr("SELECT last_name FROM  bartender WHERE card_number = '%1'").arg(number);
    q.exec(command);

    q.first();

    QString surname = q.value(0).toString();
    return surname;
}

QList<Product>* DatabaseConnector::GetProductsFromBill(QString billNumber)
{
    QSqlQuery q;
    QString command = QObject::tr("select product_name, quantity "
                                  "from order_item "
                                  "where order_number =%1").arg(billNumber);
    q.exec(command);

    QList<Product>* list = new QList<Product>;

    while(q.next())
    {   //nazwa, l szt, cena
        Product p;
        p.SetName(q.value(0).toString());
        p.SetNumber(q.value(1).toInt());

        QSqlQuery query;
        QString command2 = QObject::tr("select current_price from product where name = '%1' ").arg(q.value(0).toString());
        query.exec(command2);
        query.first();
        double price = query.value(0).toDouble();
        query.finish();

        p.SetPrice(price);

        list->append(p);
    }

    return list;
}

QList<Order> DatabaseConnector::GetOrders(QString bartenderNumber)
{
    QSqlQuery q;
    QString command = QObject::tr("SELECT order_number, customer_card_number FROM  customer_order WHERE bartender_card_number = '%1'").arg(bartenderNumber);
    q.exec(command);

    QList<Order> list;

    while(q.next())
    {   //nazwa, l szt, cena
        QString orderNumber = q.value(0).toString();
        QString customerID = q.value(1).toString();
        Order o(orderNumber);
        o.SetCustomerID(customerID);
        o.SetProductList(GetProductsFromBill(orderNumber));
        list.append(o);
    }

    return list;
}

QString DatabaseConnector::GetRandomCustomerID()
{
    QList<QString> list;
    QSqlQuery q;

    QString command = QObject::tr("SELECT card_number FROM loyal_customer");
    q.exec(command);

    while(q.next())
    {   //nazwa, l szt, cena
        QString number = q.value(0).toString();
        list.append(number);
    }

    qsrand(QTime::currentTime().msec());
    uint max = list.size();
    uint i = qrand() % max;

    return list[i];

}

bool DatabaseConnector::CustomerExists(QString number)
{
    QSqlQuery q;
    QString command = QObject::tr("SELECT card_number FROM loyal_customer WHERE card_number = '%1'").arg(number);
    q.exec(command);

    q.first();
    if(q.value(0).toString() != "") return true;
    else return false;
}

bool DatabaseConnector::SetCustomerIDinOrder(QString number, QString orderNumber)
{
    QSqlQuery q;
    return q.exec(QObject::tr("UPDATE customer_order SET customer_card_number = '%1' "
                                  "WHERE order_number = %2").arg(number).arg(orderNumber));
}



bool DatabaseConnector::RemoveProductFromOrder(QString billNumber, Product p)
{
    QSqlQuery q;
    QString command = QObject::tr("delete from`order_item` "
                                  "where `order_number` = '%1' "
                                  "and `product_name` = '%2' "
                                  "and `quantity` = '%3'").arg(billNumber).arg(p.GetName()).arg(p.GetNumber());
    q.exec(command);
    q.finish();

    command = QObject::tr("update `ingredient` set `quantity` = `quantity` + %1 "
                 "where `name` in "
                 "(select `ingredient_name` from `product_ingredient` "
                 "where `product_name` = \"%2\" )").arg(p.GetNumber()).arg(p.GetName());

    return q.exec(command);
}

bool DatabaseConnector::AddProductToOrder(QString billNumber, Product p)
{
    QSqlQuery q;
    QString command = QObject::tr("insert into `order_item` (`order_number`, `product_name`, `quantity`)"
                                  "values ( '%1', '%2' ,'%3')").arg(billNumber).arg(p.GetName()).arg(p.GetNumber());
    q.exec(command);
    q.finish();

    command = QObject::tr("update `ingredient` set `quantity` = `quantity` - %1 "
                          "where `name` in "
                          "(select `ingredient_name` from `product_ingredient` "
                          "where `product_name` = \"%2\" )").arg(p.GetNumber()).arg(p.GetName());
    return q.exec(command);
}


bool DatabaseConnector::ChangeProductNumber(QString billNumber, Product p)
{
    int available;

    QSqlQuery q;
    QString command_n = QString("update `order_item` "
                                "set quantity = %3 "
                                "where product_name = '%1' "
                                "and order_number = %2 ").arg(p.GetName()).arg(billNumber).arg(p.GetNumber());
    q.exec(command_n);
    q.first();
    available = q.value(0).toInt();

    QString command = QObject::tr("update `ingredient` set `quantity` = %1 "
                     "where `name` in "
                     "(select `ingredient_name` from `product_ingredient` "
                     "where `product_name` = \"%2\" )").arg(p.GetNumber()).arg(p.GetPrice());

    return q.exec(command);
}



bool DatabaseConnector::CloseOrder(QString billNumber)
{
    QSqlQuery q;
    QString command = QObject::tr("UPDATE customer_order SET closed= '1' "
                                  "WHERE order_number = %1").arg(billNumber);
    return q.exec(command);
}

bool DatabaseConnector::RemoveOrder(QString billNumber)
{
    QSqlQuery query;
    QString command = QObject::tr("delete from `order_item` where order_number = %1").arg(billNumber);
    query.exec(command);
    query.finish();

    command = QObject::tr("delete from `customer_order` where order_number = %1").arg(billNumber);
    return query.exec(command);
}

bool DatabaseConnector::CreateOrder(QString bartenderNumber, Order* newOrder)
{
    QString date = QDate::currentDate().toString(Qt::ISODate);

    QSqlQuery query;

    QString command = QObject::tr("INSERT INTO customer_order(bartender_card_number, order_date) VALUES('%1', '%2')"
                                  ).arg(bartenderNumber).arg(date);

    bool ok = query.exec(command);
    query.finish();

    if(ok)
    {
        command = QString("SELECT MAX(order_number) FROM customer_order");
        query.exec(command);
        query.first();
        QString *number = new QString(query.value(0).toString());
        newOrder->SetOrderNumber(*number);
    }
    return ok;
}
