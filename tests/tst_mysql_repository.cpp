#include <QtTest>
#include <QtSql>

#include "businessday.h"
#include "money.h"
#include "mysqlbarrepository.h"

// Integration test: MySqlBarRepository against a real MySQL/MariaDB server
// with schema_v2.sql loaded (its seed data included). This is the layer the
// FakeBarRepository-backed suites cannot cover: the SQL itself, the driver,
// FLOOR/MIN semantics, DECIMAL round-trips, the transaction in FinalizeOrder.
//
// Opt-in so the unit-test job stays DB-free: set BARMAN_MYSQL_TEST=1 to run,
// anything else skips. Connection comes from BARMAN_MYSQL_HOST / _DB / _USER
// / _PASSWORD (defaults match the local dev setup).
//
// The test writes only to shift / customer_order / order_item /
// ingredient_movement(reason='order') and wipes those before and after, so
// every run sees pure seed data. Seed rows themselves are never modified.

static QString envOr(const char* name, const QString& fallback)
{
    const QByteArray value = qgetenv(name);
    return value.isEmpty() ? fallback : QString::fromUtf8(value);
}

class TestMySqlRepository : public QObject
{
    Q_OBJECT

private:
    MySqlBarRepository* repo = nullptr;

    static Product productNamed(const QList<Product>& list, const QString& name)
    {
        for(const Product& p : list)
        {
            if(p.GetName() == name)
                return p;
        }
        return Product();
    }

    static int ingredientId(const QString& name)
    {
        QSqlQuery q;
        q.prepare("SELECT id FROM ingredient WHERE name = :name");
        q.bindValue(":name", name);
        q.exec();
        return q.first() ? q.value(0).toInt() : 0;
    }

    static double orderMovementTotal(int ingredient)
    {
        QSqlQuery q;
        q.prepare("SELECT COALESCE(SUM(amount), 0) FROM ingredient_movement "
                  "WHERE reason = 'order' AND ingredient_id = :id");
        q.bindValue(":id", ingredient);
        q.exec();
        return q.first() ? q.value(0).toDouble() : 0.0;
    }

    // FK order: movements reference items, items reference orders,
    // orders reference shifts.
    void wipeWrittenRows()
    {
        QSqlQuery q;
        QVERIFY(q.exec("DELETE FROM ingredient_movement WHERE reason = 'order'"));
        QVERIFY(q.exec("DELETE FROM order_item"));
        QVERIFY(q.exec("DELETE FROM customer_order"));
        QVERIFY(q.exec("DELETE FROM shift"));
    }

private slots:
    void initTestCase();
    void cleanupTestCase();

    void barCurrencyComesFromSeed();
    void employeeLookupByCard();
    void categoriesFromSeed();
    void availabilityDerivesFromLedgerAndRecipe();
    void customerLookupByCard();
    void shiftOpenAndClosePersist();
    void finalizeWritesOrderItemsAndLedgerInOneCall();
    void finalizeEmptyDraftFails();
};

void TestMySqlRepository::initTestCase()
{
    if(qgetenv("BARMAN_MYSQL_TEST") != "1")
        QSKIP("MySQL integration test skipped; set BARMAN_MYSQL_TEST=1 "
              "with a seeded barman_v2 reachable.");

    BarDatabaseConfig config;
    config.hostName = envOr("BARMAN_MYSQL_HOST", "127.0.0.1");
    config.databaseName = envOr("BARMAN_MYSQL_DB", "barman_v2");
    config.userName = envOr("BARMAN_MYSQL_USER", "bar");
    config.password = envOr("BARMAN_MYSQL_PASSWORD", "kelner2015");
    config.barName = "Boboli 8, Warszawa";

    repo = new MySqlBarRepository(config);
    QVERIFY2(repo->Open(), qPrintable(repo->LastError().text()));

    wipeWrittenRows();
}

void TestMySqlRepository::cleanupTestCase()
{
    if(repo != nullptr)
        wipeWrittenRows();
    delete repo;
}

void TestMySqlRepository::barCurrencyComesFromSeed()
{
    QCOMPARE(repo->GetBarCurrency(), QString("PLN"));
}

void TestMySqlRepository::employeeLookupByCard()
{
    Employee jan = repo->FindEmployeeByCard("8b1e9d3a-0000-4000-8000-000000000001");
    QVERIFY(jan.IsValid());
    QCOMPARE(jan.role, QString("bartender"));
    QCOMPARE(jan.firstName, QString("Jan"));
    QCOMPARE(jan.lastName, QString("Kowalski"));

    QVERIFY(!repo->FindEmployeeByCard("no-such-card").IsValid());
}

void TestMySqlRepository::categoriesFromSeed()
{
    QList<QString> categories = repo->GetCategories();
    QCOMPARE(categories, QList<QString>() << "Cocktails" << "Shots");
}

void TestMySqlRepository::availabilityDerivesFromLedgerAndRecipe()
{
    // Seed stock: 700 cl per liquid, 50 per pcs/g ingredient.
    // Availability = FLOOR(MIN(stock / recipe amount)) — computed by MySQL,
    // asserted here against hand-derived values.
    QList<Product> cocktails = repo->GetProductsFromCategory("Cocktails");
    QCOMPARE(cocktails.size(), 3);

    Product mojito = productNamed(cocktails, "Mojito");
    QVERIFY(mojito.IsValid());
    QCOMPARE(mojito.GetPrice(), Money(2200));
    QCOMPARE(mojito.GetCategory(), QString("Cocktails"));
    QCOMPARE(mojito.GetAvailable(), 6);      // mint binds: 50 g / 8 g = 6.25

    QCOMPARE(productNamed(cocktails, "Cuba Libre").GetAvailable(), 35);   // cola: 700/20
    QCOMPARE(productNamed(cocktails, "Screwdriver").GetAvailable(), 46);  // oj: 700/15

    QList<Product> shots = repo->GetProductsFromCategory("Shots");
    QCOMPARE(shots.size(), 2);
    QCOMPARE(productNamed(shots, "Rum shot").GetAvailable(), 175);        // rum: 700/4
}

void TestMySqlRepository::customerLookupByCard()
{
    Customer c = repo->FindCustomerByCard("1001");
    QVERIFY(c.IsValid());
    QCOMPARE(c.cardNumber, QString("1001"));

    QVERIFY(!repo->FindCustomerByCard("9999").IsValid());
}

void TestMySqlRepository::shiftOpenAndClosePersist()
{
    Employee jan = repo->FindEmployeeByCard("8b1e9d3a-0000-4000-8000-000000000001");

    int shiftId = repo->OpenShift(jan.id, QDateTime(QDate(2026, 7, 7), QTime(18, 0)));
    QVERIFY(shiftId > 0);

    QVERIFY(repo->CloseShift(shiftId, QDateTime(QDate(2026, 7, 8), QTime(2, 0))));

    QSqlQuery q;
    q.prepare("SELECT closed_at FROM shift WHERE id = :id");
    q.bindValue(":id", shiftId);
    QVERIFY(q.exec() && q.first());
    QCOMPARE(q.value(0).toDateTime(), QDateTime(QDate(2026, 7, 8), QTime(2, 0)));
}

void TestMySqlRepository::finalizeWritesOrderItemsAndLedgerInOneCall()
{
    Employee jan = repo->FindEmployeeByCard("8b1e9d3a-0000-4000-8000-000000000001");
    int shiftId = repo->OpenShift(jan.id, QDateTime(QDate(2026, 7, 7), QTime(18, 0)));
    QVERIFY(shiftId > 0);

    Product mojito = productNamed(repo->GetProductsFromCategory("Cocktails"), "Mojito");
    Product rumShot = productNamed(repo->GetProductsFromCategory("Shots"), "Rum shot");
    QVERIFY(mojito.IsValid() && rumShot.IsValid());

    Customer customer = repo->FindCustomerByCard("1001");
    QVERIFY(customer.IsValid());

    DraftOrder draft("1");
    draft.AddProduct(mojito, 2);
    draft.AddProduct(rumShot, 1);
    draft.SetCustomer(customer.id, customer.cardNumber, 0.1);

    // Sold at 01:30 — the business day is still the previous calendar day.
    QDateTime soldAt(QDate(2026, 7, 8), QTime(1, 30));
    QVERIFY(repo->FinalizeOrder(draft, shiftId, businessDay(soldAt), soldAt));

    // The order row: copies of discount rate and currency, stamped business day.
    QSqlQuery q;
    QVERIFY(q.exec("SELECT shift_id, business_day, customer_id, discount_rate, currency "
                   "FROM customer_order") && q.first());
    QCOMPARE(q.value(0).toInt(), shiftId);
    QCOMPARE(q.value(1).toDate(), QDate(2026, 7, 7));
    QCOMPARE(q.value(2).toInt(), customer.id);
    QVERIFY(qFuzzyCompare(q.value(3).toDouble(), 0.1));
    QCOMPARE(q.value(4).toString(), QString("PLN"));
    QVERIFY(!q.next());                       // exactly one order

    // The item rows: name and price copied at sale time.
    q.prepare("SELECT product_name, unit_price, quantity FROM order_item "
              "WHERE product_id = :id");
    q.bindValue(":id", mojito.GetId());
    QVERIFY(q.exec() && q.first());
    QCOMPARE(q.value(0).toString(), QString("Mojito"));
    QCOMPARE(moneyFromDecimalString(q.value(1).toString()), Money(2200));
    QCOMPARE(q.value(2).toInt(), 2);

    // The ledger: -(recipe amount x quantity) per line-ingredient.
    // Mojito x2: rum -8, lime -2, mint -16; Rum shot x1: rum -4.
    QVERIFY(q.exec("SELECT COUNT(*) FROM ingredient_movement WHERE reason = 'order'")
            && q.first());
    QCOMPARE(q.value(0).toInt(), 4);
    QCOMPARE(orderMovementTotal(ingredientId("rum")), -12.0);
    QCOMPARE(orderMovementTotal(ingredientId("lime")), -2.0);
    QCOMPARE(orderMovementTotal(ingredientId("mint")), -16.0);
    QCOMPARE(orderMovementTotal(ingredientId("vodka")), 0.0);

    // End to end: the sale lowers derived availability on the next query.
    // Mojito now binds on mint: (50 - 16) / 8 = 4.25 -> 4.
    Product mojitoAfter = productNamed(repo->GetProductsFromCategory("Cocktails"), "Mojito");
    QCOMPARE(mojitoAfter.GetAvailable(), 4);

    wipeWrittenRows();
}

void TestMySqlRepository::finalizeEmptyDraftFails()
{
    DraftOrder empty("2");
    QVERIFY(!repo->FinalizeOrder(empty, 1, QDate(2026, 7, 7),
                                 QDateTime(QDate(2026, 7, 7), QTime(20, 0))));
}

QTEST_GUILESS_MAIN(TestMySqlRepository)
#include "tst_mysql_repository.moc"
