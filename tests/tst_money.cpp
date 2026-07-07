#include <QtTest>

#include "money.h"

class TestMoney : public QObject
{
    Q_OBJECT

private slots:
    void parsesDecimalStrings();
    void formatsDecimalStrings();
    void roundTripsThroughFormatting();
    void appliesDiscountWithRoundingHalfAwayFromZero();
};

void TestMoney::parsesDecimalStrings()
{
    QCOMPARE(moneyFromDecimalString("22.00"), Money(2200));
    QCOMPARE(moneyFromDecimalString("0.05"), Money(5));
    QCOMPARE(moneyFromDecimalString("0"), Money(0));
    QCOMPARE(moneyFromDecimalString("19.99"), Money(1999));
}

void TestMoney::formatsDecimalStrings()
{
    QCOMPARE(moneyToDecimalString(2200), QString("22.00"));
    QCOMPARE(moneyToDecimalString(5), QString("0.05"));
    QCOMPARE(moneyToDecimalString(0), QString("0.00"));
    QCOMPARE(moneyToDecimalString(1999), QString("19.99"));
    QCOMPARE(moneyToDecimalString(-150), QString("-1.50"));
}

void TestMoney::roundTripsThroughFormatting()
{
    for(Money m = 0; m < 500; ++m)
        QCOMPARE(moneyFromDecimalString(moneyToDecimalString(m)), m);
}

void TestMoney::appliesDiscountWithRoundingHalfAwayFromZero()
{
    QCOMPARE(applyDiscount(1000, 0.1), Money(900));
    QCOMPARE(applyDiscount(1000, 0.0), Money(1000));
    // 1005 * 0.9 = 904.5 -> rounds away from zero
    QCOMPARE(applyDiscount(1005, 0.1), Money(905));
    // 333 * 0.9 = 299.7
    QCOMPARE(applyDiscount(333, 0.1), Money(300));
    QCOMPARE(applyDiscount(0, 0.1), Money(0));
}

QTEST_APPLESS_MAIN(TestMoney)
#include "tst_money.moc"
