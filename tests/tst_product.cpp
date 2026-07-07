#include <QtTest>

#include "product.h"

class TestProduct : public QObject
{
    Q_OBJECT

private slots:
    void constructorStoresValues();
    void defaultConstructedProductIsInvalid();
};

void TestProduct::constructorStoresValues()
{
    Product p(3, "Mojito", 2200, "Cocktails", 12);
    QCOMPARE(p.GetId(), 3);
    QCOMPARE(p.GetName(), QString("Mojito"));
    QCOMPARE(p.GetPrice(), Money(2200));
    QCOMPARE(p.GetCategory(), QString("Cocktails"));
    QCOMPARE(p.GetAvailable(), 12);
    QVERIFY(p.IsValid());
}

void TestProduct::defaultConstructedProductIsInvalid()
{
    Product p;
    QVERIFY(!p.IsValid());
    QCOMPARE(p.GetPrice(), Money(0));
    QCOMPARE(p.GetAvailable(), 0);
}

QTEST_APPLESS_MAIN(TestProduct)
#include "tst_product.moc"
