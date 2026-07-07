#include <QtTest>

#include "order.h"

class TestOrder : public QObject
{
    Q_OBJECT

private slots:
    void newOrderIsEmpty();
    void addProductAppendsToList();
    void removeProductRemovesMatchingEntry();
    void removeNonExistentProductLeavesListUntouched();
    void changeProductNumberUpdatesQuantity();
    void changeProductNumberOfUnknownProductDoesNothing();
    void copyIsIndependentOfOriginal();
    void setProductListReplacesContents();
    void equalityComparesOrderNumber();
};

void TestOrder::newOrderIsEmpty()
{
    Order o("Bill_1");
    QCOMPARE(o.GetOrderNumber(), QString("Bill_1"));
    QCOMPARE(o.GetCustomerID(), QString(""));
    QCOMPARE(o.GetProductList().size(), 0);
}

void TestOrder::addProductAppendsToList()
{
    Order o("Bill_1");
    o.AddProduct(Product("Mojito", 2, 15.5));
    QCOMPARE(o.GetProductList().size(), 1);
    QCOMPARE(o.GetProductList().first().GetName(), QString("Mojito"));
}

void TestOrder::removeProductRemovesMatchingEntry()
{
    Order o("Bill_1");
    o.AddProduct(Product("Mojito", 2, 15.5));
    o.AddProduct(Product("Cuba Libre", 1, 18.0));

    o.RemoveProduct(Product("Mojito", 2, 15.5));

    QCOMPARE(o.GetProductList().size(), 1);
    QCOMPARE(o.GetProductList().first().GetName(), QString("Cuba Libre"));
}

void TestOrder::removeNonExistentProductLeavesListUntouched()
{
    Order o("Bill_1");
    o.AddProduct(Product("Mojito", 2, 15.5));

    o.RemoveProduct(Product("Cuba Libre", 1, 18.0));

    QCOMPARE(o.GetProductList().size(), 1);
}

void TestOrder::changeProductNumberUpdatesQuantity()
{
    Order o("Bill_1");
    o.AddProduct(Product("Mojito", 2, 15.5));

    o.ChangeProductNumber(Product("Mojito", 2, 15.5), 5);

    QCOMPARE(o.GetProductList().first().GetNumber(), 5u);
}

void TestOrder::changeProductNumberOfUnknownProductDoesNothing()
{
    Order o("Bill_1");
    o.AddProduct(Product("Mojito", 2, 15.5));

    // Product not on the order: quantity 3 does not match, so indexOf misses.
    o.ChangeProductNumber(Product("Mojito", 3, 15.5), 7);

    QCOMPARE(o.GetProductList().size(), 1);
    QCOMPARE(o.GetProductList().first().GetNumber(), 2u);
}

void TestOrder::copyIsIndependentOfOriginal()
{
    Order original("Bill_1");
    original.AddProduct(Product("Mojito", 2, 15.5));

    Order copy = original;
    copy.AddProduct(Product("Cuba Libre", 1, 18.0));
    copy.ChangeProductNumber(Product("Mojito", 2, 15.5), 9);

    QCOMPARE(original.GetProductList().size(), 1);
    QCOMPARE(original.GetProductList().first().GetNumber(), 2u);
    QCOMPARE(copy.GetProductList().size(), 2);
}

void TestOrder::setProductListReplacesContents()
{
    Order o("Bill_1");
    o.AddProduct(Product("Mojito", 2, 15.5));

    QList<Product> replacement;
    replacement.append(Product("Cuba Libre", 1, 18.0));
    o.SetProductList(replacement);

    QCOMPARE(o.GetProductList().size(), 1);
    QCOMPARE(o.GetProductList().first().GetName(), QString("Cuba Libre"));
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
