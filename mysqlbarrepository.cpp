#include "mysqlbarrepository.h"
#include <QDate>


MySqlBarRepository::MySqlBarRepository(const BarDatabaseConfig& config)
{
    _config = config;
    db = QSqlDatabase::addDatabase("QMYSQL");
}

MySqlBarRepository::~MySqlBarRepository()
{
}

bool MySqlBarRepository::Open()
{
    db.setHostName(_config.hostName);
    db.setDatabaseName(_config.databaseName);
    db.setUserName(_config.userName);
    db.setPassword(_config.password);

    return db.open();
}


QList<Product> MySqlBarRepository::GetProductsFromCategory(QString category)
{
    QList<Product> list;
    QSqlQuery q(db);

    q.prepare("select p.name, current_price, min(i.quantity) from bar_product "
              "join product as p on p.name = bar_product.product_name "
              "join product_ingredient as p_i on p.name = p_i.product_name "
              "join ingredient as i on p_i.ingredient_name = i.name  "
              "where category = :category and bar_id = :bar "
              "group by p.name ");
    q.bindValue(":category", category);
    q.bindValue(":bar", _config.barName);
    q.exec();

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

QList<QString> MySqlBarRepository::GetCategories()
{
    QList<QString> list;
    QSqlQuery q(db);
    q.prepare("SELECT DISTINCT category FROM "
              "bar_product as b join product as p "
              "on b.product_name = p.name "
              "WHERE b.bar_id = :bar order by category");
    q.bindValue(":bar", _config.barName);
    q.exec();

    while(q.next())
        list.append(q.value(0).toString());
    return list;
}

bool MySqlBarRepository::BartenderExists(QString cardNumber)
{
    QSqlQuery q(db);
    q.prepare("SELECT card_number FROM bartender WHERE card_number = :card");
    q.bindValue(":card", cardNumber);
    q.exec();

    return q.first();
}

QList<QString> MySqlBarRepository::GetBartenderCardNumbers()
{
    QSqlQuery q(db);
    q.exec("SELECT card_number FROM bartender");

    QList<QString> list;

    while(q.next())
        list.append(q.value(0).toString());

    return list;
}

QString MySqlBarRepository::GetBartenderName(QString cardNumber)
{
    QSqlQuery q(db);
    q.prepare("SELECT first_name FROM bartender WHERE card_number = :card");
    q.bindValue(":card", cardNumber);
    q.exec();

    if(!q.first())
        return QString("");

    return q.value(0).toString();
}

QString MySqlBarRepository::GetBartenderSurname(QString cardNumber)
{
    QSqlQuery q(db);
    q.prepare("SELECT last_name FROM bartender WHERE card_number = :card");
    q.bindValue(":card", cardNumber);
    q.exec();

    if(!q.first())
        return QString("");

    return q.value(0).toString();
}

QList<Product> MySqlBarRepository::GetProductsFromBill(QString billNumber)
{
    QSqlQuery q(db);
    q.prepare("select o.product_name, o.quantity, p.current_price "
              "from order_item as o "
              "join product as p on p.name = o.product_name "
              "where o.order_number = :order");
    q.bindValue(":order", billNumber);
    q.exec();

    QList<Product> list;

    while(q.next())
    {
        Product p;
        p.SetName(q.value(0).toString());
        p.SetNumber(q.value(1).toInt());
        p.SetPrice(q.value(2).toDouble());

        list.append(p);
    }

    return list;
}

QList<Order> MySqlBarRepository::GetOrders(QString bartenderNumber)
{
    QSqlQuery q(db);
    q.prepare("SELECT order_number, customer_card_number FROM customer_order "
              "WHERE bartender_card_number = :bartender");
    q.bindValue(":bartender", bartenderNumber);
    q.exec();

    QList<Order> list;

    while(q.next())
    {
        QString orderNumber = q.value(0).toString();
        QString customerID = q.value(1).toString();
        Order o(orderNumber);
        o.SetCustomerID(customerID);
        o.SetProductList(GetProductsFromBill(orderNumber));
        list.append(o);
    }

    return list;
}

QList<QString> MySqlBarRepository::GetCustomerCardNumbers()
{
    QList<QString> list;
    QSqlQuery q(db);

    q.exec("SELECT card_number FROM loyal_customer");

    while(q.next())
        list.append(q.value(0).toString());

    return list;
}

bool MySqlBarRepository::CustomerExists(QString cardNumber)
{
    QSqlQuery q(db);
    q.prepare("SELECT card_number FROM loyal_customer WHERE card_number = :card");
    q.bindValue(":card", cardNumber);
    q.exec();

    return q.first();
}

bool MySqlBarRepository::SetCustomerIDinOrder(QString cardNumber, QString orderNumber)
{
    QSqlQuery q(db);
    q.prepare("UPDATE customer_order SET customer_card_number = :card "
              "WHERE order_number = :order");
    q.bindValue(":card", cardNumber);
    q.bindValue(":order", orderNumber);
    return q.exec();
}



bool MySqlBarRepository::RemoveProductFromOrder(QString billNumber, Product p)
{
    if(!db.transaction())
        return false;

    // Read the stored quantity: stock has to be restored by what the DB
    // actually holds, not by the (possibly stale) UI value.
    QSqlQuery q(db);
    q.prepare("select quantity from order_item "
              "where order_number = :order and product_name = :product");
    q.bindValue(":order", billNumber);
    q.bindValue(":product", p.GetName());

    if(!q.exec() || !q.first())
    {
        db.rollback();
        return false;
    }

    int storedQuantity = q.value(0).toInt();

    q.prepare("delete from order_item "
              "where order_number = :order and product_name = :product");
    q.bindValue(":order", billNumber);
    q.bindValue(":product", p.GetName());

    if(!q.exec())
    {
        db.rollback();
        return false;
    }

    q.prepare("update ingredient set quantity = quantity + :amount "
              "where name in "
              "(select ingredient_name from product_ingredient "
              "where product_name = :product)");
    q.bindValue(":amount", storedQuantity);
    q.bindValue(":product", p.GetName());

    if(!q.exec())
    {
        db.rollback();
        return false;
    }

    return db.commit();
}

bool MySqlBarRepository::AddProductToOrder(QString billNumber, Product p)
{
    if(!db.transaction())
        return false;

    QSqlQuery q(db);
    q.prepare("insert into order_item (order_number, product_name, quantity) "
              "values (:order, :product, :quantity)");
    q.bindValue(":order", billNumber);
    q.bindValue(":product", p.GetName());
    q.bindValue(":quantity", p.GetNumber());

    if(!q.exec())
    {
        db.rollback();
        return false;
    }

    // v1 has no recipe amounts, so stock is decremented by the order quantity
    // for every ingredient of the product; schema v2 fixes the model.
    q.prepare("update ingredient set quantity = quantity - :amount "
              "where name in "
              "(select ingredient_name from product_ingredient "
              "where product_name = :product)");
    q.bindValue(":amount", p.GetNumber());
    q.bindValue(":product", p.GetName());

    if(!q.exec())
    {
        db.rollback();
        return false;
    }

    return db.commit();
}


bool MySqlBarRepository::ChangeProductNumber(QString billNumber, Product p)
{
    if(!db.transaction())
        return false;

    QSqlQuery q(db);
    q.prepare("select quantity from order_item "
              "where order_number = :order and product_name = :product");
    q.bindValue(":order", billNumber);
    q.bindValue(":product", p.GetName());

    if(!q.exec() || !q.first())
    {
        db.rollback();
        return false;
    }

    int oldQuantity = q.value(0).toInt();
    int newQuantity = p.GetNumber();

    q.prepare("update order_item set quantity = :quantity "
              "where order_number = :order and product_name = :product");
    q.bindValue(":quantity", newQuantity);
    q.bindValue(":order", billNumber);
    q.bindValue(":product", p.GetName());

    if(!q.exec())
    {
        db.rollback();
        return false;
    }

    // Adjust stock by the difference instead of overwriting it.
    q.prepare("update ingredient set quantity = quantity + :amount "
              "where name in "
              "(select ingredient_name from product_ingredient "
              "where product_name = :product)");
    q.bindValue(":amount", oldQuantity - newQuantity);
    q.bindValue(":product", p.GetName());

    if(!q.exec())
    {
        db.rollback();
        return false;
    }

    return db.commit();
}



bool MySqlBarRepository::CloseOrder(QString billNumber)
{
    QSqlQuery q(db);
    q.prepare("UPDATE customer_order SET closed = '1' WHERE order_number = :order");
    q.bindValue(":order", billNumber);
    return q.exec();
}

bool MySqlBarRepository::RemoveOrder(QString billNumber)
{
    if(!db.transaction())
        return false;

    QSqlQuery q(db);
    q.prepare("delete from order_item where order_number = :order");
    q.bindValue(":order", billNumber);

    if(!q.exec())
    {
        db.rollback();
        return false;
    }

    q.prepare("delete from customer_order where order_number = :order");
    q.bindValue(":order", billNumber);

    if(!q.exec())
    {
        db.rollback();
        return false;
    }

    return db.commit();
}

bool MySqlBarRepository::CreateOrder(QString bartenderNumber, Order* newOrder)
{
    QString date = QDate::currentDate().toString(Qt::ISODate);

    QSqlQuery q(db);
    q.prepare("INSERT INTO customer_order(bartender_card_number, order_date) "
              "VALUES(:bartender, :date)");
    q.bindValue(":bartender", bartenderNumber);
    q.bindValue(":date", date);

    if(!q.exec())
        return false;

    newOrder->SetOrderNumber(q.lastInsertId().toString());
    return true;
}
