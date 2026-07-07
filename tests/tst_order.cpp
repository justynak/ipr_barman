#include <QtTest>

#include "order.h"

class TestOrder : public QObject
{
    Q_OBJECT

private slots:
    void newOrderIsEmpty();
    void addProductAppendsToList();
    void removeProductRemovesMatchingEntry();
    void equalityComparesOrderNumber();
};

void TestOrder::newOrderIsEmpty()
{
    Order o("Bill_1");
    QCOMPARE(o.GetOrderNumber(), QString("Bill_1"));
    QCOMPARE(o.GetCustomerID(), QString(""));
    QVERIFY(o.GetProductList() != NULL);
    QCOMPARE(o.GetProductList()->size(), 0);
}

void TestOrder::addProductAppendsToList()
{
    Order o("Bill_1");
    o.AddProduct(Product("Mojito", 2, 15.5));
    QCOMPARE(o.GetProductList()->size(), 1);
    QCOMPARE(o.GetProductList()->first().GetName(), QString("Mojito"));
}

void TestOrder::removeProductRemovesMatchingEntry()
{
    Order o("Bill_1");
    o.AddProduct(Product("Mojito", 2, 15.5));
    o.AddProduct(Product("Cuba Libre", 1, 18.0));

    o.RemoveProduct(Product("Mojito", 2, 15.5));

    QCOMPARE(o.GetProductList()->size(), 1);
    QCOMPARE(o.GetProductList()->first().GetName(), QString("Cuba Libre"));
}

void TestOrder::equalityComparesOrderNumber()
{
    Order a("Bill_1");
    Order b("Bill_1");
    Order c("Bill_2");
    QVERIFY(a == b);
    QVERIFY(!(a == c));
}

QTEST_APPLESS_MAIN(TestOrder)
#include "tst_order.moc"
