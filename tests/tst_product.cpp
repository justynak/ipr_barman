#include <QtTest>

#include "product.h"

class TestProduct : public QObject
{
    Q_OBJECT

private slots:
    void constructorStoresValues();
    void settersOverwriteValues();
    void equalityComparesNameAndNumber();
};

void TestProduct::constructorStoresValues()
{
    Product p("Mojito", 2, 15.5);
    QCOMPARE(p.GetName(), QString("Mojito"));
    QCOMPARE(p.GetNumber(), 2u);
    QCOMPARE(p.GetPrice(), 15.5);
}

void TestProduct::settersOverwriteValues()
{
    Product p("Mojito", 2, 15.5);
    QVERIFY(p.SetName("Cuba Libre"));
    QVERIFY(p.SetNumber(3));
    QVERIFY(p.SetPrice(18.0));
    QCOMPARE(p.GetName(), QString("Cuba Libre"));
    QCOMPARE(p.GetNumber(), 3u);
    QCOMPARE(p.GetPrice(), 18.0);
}

void TestProduct::equalityComparesNameAndNumber()
{
    Product a("Mojito", 2, 15.5);
    Product samePriceIgnored("Mojito", 2, 99.0);
    Product otherName("Cuba Libre", 2, 15.5);
    Product otherNumber("Mojito", 3, 15.5);

    QVERIFY(a == samePriceIgnored);
    QVERIFY(!(a == otherName));
    QVERIFY(!(a == otherNumber));
}

QTEST_APPLESS_MAIN(TestProduct)
#include "tst_product.moc"
