#include <QtTest>

#include "draftorder.h"

class TestDraftOrder : public QObject
{
    Q_OBJECT

private:
    Product mojito() { return Product(1, "Mojito", 2200, "Cocktails", 10); }
    Product cubaLibre() { return Product(2, "Cuba Libre", 2000, "Cocktails", 10); }

private slots:
    void newDraftIsEmpty();
    void addProductCreatesLineWithCopies();
    void addingSameProductMergesQuantities();
    void addingZeroQuantityDoesNothing();
    void changeQuantityUpdatesLine();
    void changeQuantityToZeroRemovesLine();
    void changeQuantityOfUnknownProductFails();
    void removeProduct();
    void subtotalSumsLines();
    void totalAppliesDiscountRate();
    void customerCopiesCardAndRate();
};

void TestDraftOrder::newDraftIsEmpty()
{
    DraftOrder d("7");
    QCOMPARE(d.GetLabel(), QString("7"));
    QVERIFY(d.IsEmpty());
    QVERIFY(!d.HasCustomer());
    QCOMPARE(d.GetDiscountRate(), 0.0);
    QCOMPARE(d.Subtotal(), Money(0));
    QCOMPARE(d.Total(), Money(0));
}

void TestDraftOrder::addProductCreatesLineWithCopies()
{
    DraftOrder d("1");
    d.AddProduct(mojito(), 2);

    QCOMPARE(d.GetLines().size(), 1);
    OrderLine line = d.GetLines().first();
    QCOMPARE(line.productId, 1);
    QCOMPARE(line.productName, QString("Mojito"));
    QCOMPARE(line.unitPrice, Money(2200));
    QCOMPARE(line.quantity, 2);
}

void TestDraftOrder::addingSameProductMergesQuantities()
{
    DraftOrder d("1");
    d.AddProduct(mojito(), 2);
    d.AddProduct(mojito(), 3);

    QCOMPARE(d.GetLines().size(), 1);
    QCOMPARE(d.GetLines().first().quantity, 5);
}

void TestDraftOrder::addingZeroQuantityDoesNothing()
{
    DraftOrder d("1");
    d.AddProduct(mojito(), 0);
    d.AddProduct(mojito(), -2);

    QVERIFY(d.IsEmpty());
}

void TestDraftOrder::changeQuantityUpdatesLine()
{
    DraftOrder d("1");
    d.AddProduct(mojito(), 2);

    QVERIFY(d.ChangeQuantity(1, 7));
    QCOMPARE(d.GetLines().first().quantity, 7);
}

void TestDraftOrder::changeQuantityToZeroRemovesLine()
{
    DraftOrder d("1");
    d.AddProduct(mojito(), 2);

    QVERIFY(d.ChangeQuantity(1, 0));
    QVERIFY(d.IsEmpty());
}

void TestDraftOrder::changeQuantityOfUnknownProductFails()
{
    DraftOrder d("1");
    d.AddProduct(mojito(), 2);

    QVERIFY(!d.ChangeQuantity(99, 5));
    QCOMPARE(d.GetLines().first().quantity, 2);
}

void TestDraftOrder::removeProduct()
{
    DraftOrder d("1");
    d.AddProduct(mojito(), 2);
    d.AddProduct(cubaLibre(), 1);

    QVERIFY(d.RemoveProduct(1));
    QCOMPARE(d.GetLines().size(), 1);
    QCOMPARE(d.GetLines().first().productId, 2);

    QVERIFY(!d.RemoveProduct(99));
}

void TestDraftOrder::subtotalSumsLines()
{
    DraftOrder d("1");
    d.AddProduct(mojito(), 2);      // 2 x 22.00
    d.AddProduct(cubaLibre(), 1);   // 1 x 20.00

    QCOMPARE(d.Subtotal(), Money(6400));
}

void TestDraftOrder::totalAppliesDiscountRate()
{
    DraftOrder d("1");
    d.AddProduct(mojito(), 2);
    d.AddProduct(cubaLibre(), 1);
    d.SetCustomer(5, "1001", 0.1);

    QCOMPARE(d.Subtotal(), Money(6400));
    QCOMPARE(d.Total(), Money(5760));
}

void TestDraftOrder::customerCopiesCardAndRate()
{
    DraftOrder d("1");
    d.SetCustomer(5, "1001", LOYAL_CUSTOMER_DISCOUNT);

    QVERIFY(d.HasCustomer());
    QCOMPARE(d.GetCustomerId(), 5);
    QCOMPARE(d.GetCustomerCard(), QString("1001"));
    QCOMPARE(d.GetDiscountRate(), LOYAL_CUSTOMER_DISCOUNT);
}

QTEST_APPLESS_MAIN(TestDraftOrder)
#include "tst_draftorder.moc"
