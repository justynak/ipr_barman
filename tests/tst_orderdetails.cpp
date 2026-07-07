#include <QtTest>

#include "orderdetails.h"

class TestOrderDetails : public QObject
{
    Q_OBJECT

private slots:
    void costIsSumOfQuantityTimesPrice();
    void costOfEmptyOrderIsZero();
    void recalculateCostReflectsMutation();
};

void TestOrderDetails::costIsSumOfQuantityTimesPrice()
{
    Order o("Bill_1");
    o.AddProduct(Product("Mojito", 2, 15.5));
    o.AddProduct(Product("Cuba Libre", 1, 18.0));

    OrderDetails details(&o);

    QCOMPARE(details.GetCost(), 2 * 15.5 + 1 * 18.0);
    QCOMPARE(details.GetOrderNumber(), QString("Bill_1"));
}

void TestOrderDetails::costOfEmptyOrderIsZero()
{
    Order o("Bill_1");
    OrderDetails details(&o);
    QCOMPARE(details.GetCost(), 0.0);
}

void TestOrderDetails::recalculateCostReflectsMutation()
{
    Order o("Bill_1");
    o.AddProduct(Product("Mojito", 2, 15.5));
    OrderDetails details(&o);

    o.AddProduct(Product("Cuba Libre", 1, 18.0));
    details.RecalculateCost();

    QCOMPARE(details.GetCost(), 2 * 15.5 + 1 * 18.0);
}

QTEST_APPLESS_MAIN(TestOrderDetails)
#include "tst_orderdetails.moc"
