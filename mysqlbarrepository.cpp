#include "mysqlbarrepository.h"


MySqlBarRepository::MySqlBarRepository(const BarDatabaseConfig& config)
    : _config(config),
      db(QSqlDatabase::addDatabase("QMYSQL"))
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

QString MySqlBarRepository::GetBarCurrency()
{
    QSqlQuery q(db);
    q.prepare("SELECT currency FROM bar WHERE name = :bar");
    q.bindValue(":bar", _config.barName);
    q.exec();

    if(!q.first())
        return QString("");
    return q.value(0).toString();
}

Employee MySqlBarRepository::FindEmployeeByCard(QString cardNumber)
{
    QSqlQuery q(db);
    q.prepare("SELECT id, role, first_name, last_name FROM employee "
              "WHERE card_number = :card");
    q.bindValue(":card", cardNumber);
    q.exec();

    Employee e;
    if(!q.first())
        return e;

    e.id = q.value(0).toInt();
    e.role = q.value(1).toString();
    e.cardNumber = cardNumber;
    e.firstName = q.value(2).toString();
    e.lastName = q.value(3).toString();
    return e;
}

QList<QString> MySqlBarRepository::GetEmployeeCardNumbers()
{
    QSqlQuery q(db);
    q.exec("SELECT card_number FROM employee");

    QList<QString> list;
    while(q.next())
        list.append(q.value(0).toString());
    return list;
}

int MySqlBarRepository::OpenShift(int employeeId, QDateTime openedAt)
{
    QSqlQuery q(db);
    q.prepare("INSERT INTO shift(employee_id, opened_at) VALUES(:employee, :opened)");
    q.bindValue(":employee", employeeId);
    q.bindValue(":opened", openedAt);

    if(!q.exec())
        return 0;
    return q.lastInsertId().toInt();
}

bool MySqlBarRepository::CloseShift(int shiftId, QDateTime closedAt)
{
    QSqlQuery q(db);
    q.prepare("UPDATE shift SET closed_at = :closed WHERE id = :id");
    q.bindValue(":closed", closedAt);
    q.bindValue(":id", shiftId);
    return q.exec();
}

QList<QString> MySqlBarRepository::GetCategories()
{
    QList<QString> list;
    QSqlQuery q(db);
    q.prepare("SELECT DISTINCT p.category FROM bar_product bp "
              "JOIN bar b ON b.id = bp.bar_id "
              "JOIN product p ON p.id = bp.product_id "
              "WHERE b.name = :bar ORDER BY p.category");
    q.bindValue(":bar", _config.barName);
    q.exec();

    while(q.next())
        list.append(q.value(0).toString());
    return list;
}

QList<Product> MySqlBarRepository::GetProductsFromCategory(QString category)
{
    QList<Product> list;
    QSqlQuery q(db);

    // Availability = floor(min(stock / recipe amount)) over the product's
    // ingredients; stock comes from the ledger view, never from a counter.
    q.prepare("SELECT p.id, p.name, p.current_price, "
              "       FLOOR(MIN(s.stock / pi.amount)) "
              "FROM bar_product bp "
              "JOIN bar b ON b.id = bp.bar_id "
              "JOIN product p ON p.id = bp.product_id "
              "JOIN product_ingredient pi ON pi.product_id = p.id "
              "JOIN ingredient_stock s ON s.id = pi.ingredient_id "
              "WHERE p.category = :category AND b.name = :bar "
              "GROUP BY p.id, p.name, p.current_price");
    q.bindValue(":category", category);
    q.bindValue(":bar", _config.barName);
    q.exec();

    while(q.next())
    {
        int available = q.value(3).toInt();
        if(available < 0)
            available = 0;

        list.append(Product(q.value(0).toInt(),
                            q.value(1).toString(),
                            moneyFromDecimalString(q.value(2).toString()),
                            category,
                            available));
    }
    return list;
}

Customer MySqlBarRepository::FindCustomerByCard(QString cardNumber)
{
    QSqlQuery q(db);
    q.prepare("SELECT id FROM customer WHERE card_number = :card");
    q.bindValue(":card", cardNumber);
    q.exec();

    Customer c;
    if(!q.first())
        return c;

    c.id = q.value(0).toInt();
    c.cardNumber = cardNumber;
    return c;
}

QList<QString> MySqlBarRepository::GetCustomerCardNumbers()
{
    QList<QString> list;
    QSqlQuery q(db);

    q.exec("SELECT card_number FROM customer");

    while(q.next())
        list.append(q.value(0).toString());

    return list;
}

bool MySqlBarRepository::FinalizeOrder(const DraftOrder& draft, int shiftId,
                                       QDate businessDay, QDateTime createdAt)
{
    if(draft.IsEmpty())
        return false;

    QString currency = GetBarCurrency();

    if(!db.transaction())
        return false;

    QSqlQuery q(db);
    q.prepare("INSERT INTO customer_order"
              "(shift_id, business_day, customer_id, discount_rate, currency, created_at) "
              "VALUES(:shift, :day, :customer, :rate, :currency, :created)");
    q.bindValue(":shift", shiftId);
    q.bindValue(":day", businessDay);
    q.bindValue(":customer", draft.HasCustomer() ? QVariant(draft.GetCustomerId())
                                                 : QVariant(QVariant::Int));
    q.bindValue(":rate", draft.GetDiscountRate());
    q.bindValue(":currency", currency);
    q.bindValue(":created", createdAt);

    if(!q.exec())
    {
        db.rollback();
        return false;
    }

    int orderId = q.lastInsertId().toInt();

    const QList<OrderLine> lines = draft.GetLines();
    for(const OrderLine& line : lines)
    {
        q.prepare("INSERT INTO order_item"
                  "(order_id, product_id, product_name, unit_price, quantity) "
                  "VALUES(:order, :product, :name, :price, :quantity)");
        q.bindValue(":order", orderId);
        q.bindValue(":product", line.productId);
        q.bindValue(":name", line.productName);
        q.bindValue(":price", moneyToDecimalString(line.unitPrice));
        q.bindValue(":quantity", line.quantity);

        if(!q.exec())
        {
            db.rollback();
            return false;
        }

        int orderItemId = q.lastInsertId().toInt();

        // One negative ledger movement per line-ingredient:
        // -(recipe amount × quantity), reason 'order'.
        q.prepare("INSERT INTO ingredient_movement"
                  "(ingredient_id, amount, reason, order_item_id, occurred_at) "
                  "SELECT pi.ingredient_id, -(pi.amount * :quantity), 'order', :item, :occurred "
                  "FROM product_ingredient pi WHERE pi.product_id = :product");
        q.bindValue(":quantity", line.quantity);
        q.bindValue(":item", orderItemId);
        q.bindValue(":occurred", createdAt);
        q.bindValue(":product", line.productId);

        if(!q.exec())
        {
            db.rollback();
            return false;
        }
    }

    return db.commit();
}
