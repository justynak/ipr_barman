#include <QtTest>

#include "orderdetails.h"

class TestOrderDetails : public QObject
{
    Q_OBJECT

private slots:
    void defaultConstructedDetailsAreEmpty();
    void costIsSumOfQuantityTimesPrice();
    void costOfEmptyOrderIsZero();
    void recalculateCostReflectsMutation();
    void noDiscountWithoutCustomer();
    void discountForOrderWithCustomer();
    void discountedCostAppliesDiscountRate();
};

void TestOrderDetails::defaultConstructedDetailsAreEmpty()
{
    OrderDetails details;
    QCOMPARE(details.GetCost(), 0.0);
    QCOMPARE(details.GetDiscount(), 0.0);
    QCOMPARE(details.GetOrderNumber(), QString(""));
    QCOMPARE(details.GetProductList().size(), 0);

    details.RecalculateCost();
    QCOMPARE(details.GetCost(), 0.0);
}

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

void TestOrderDetails::noDiscountWithoutCustomer()
{
    Order o("Bill_1");
    o.AddProduct(Product("Mojito", 2, 15.5));

    OrderDetails details(&o);

    QCOMPARE(details.GetDiscount(), 0.0);
    QCOMPARE(details.GetDiscountedCost(), details.GetCost());
}

void TestOrderDetails::discountForOrderWithCustomer()
{
    Order o("Bill_1");
    o.SetCustomerID("C123");

    OrderDetails details(&o);

    QCOMPARE(details.GetDiscount(), LOYAL_CUSTOMER_DISCOUNT);
}

void TestOrderDetails::discountedCostAppliesDiscountRate()
{
    Order o("Bill_1");
    o.SetCustomerID("C123");
    o.AddProduct(Product("Mojito", 2, 15.5));
    o.AddProduct(Product("Cuba Libre", 1, 18.0));

    OrderDetails details(&o);

    double cost = 2 * 15.5 + 1 * 18.0;
    QCOMPARE(details.GetCost(), cost);
    QCOMPARE(details.GetDiscountedCost(), cost * (1.0 - LOYAL_CUSTOMER_DISCOUNT));
}

QTEST_APPLESS_MAIN(TestOrderDetails)
#include "tst_orderdetails.moc"
