#include <QtTest>

#include "businessday.h"

class TestBusinessDay : public QObject
{
    Q_OBJECT

private slots:
    void eveningBelongsToTheSameDay();
    void afterMidnightBelongsToThePreviousDay();
    void cutoffBoundary();
    void customCutoffHour();
};

void TestBusinessDay::eveningBelongsToTheSameDay()
{
    QDateTime evening(QDate(2026, 7, 7), QTime(23, 30));
    QCOMPARE(businessDay(evening), QDate(2026, 7, 7));
}

void TestBusinessDay::afterMidnightBelongsToThePreviousDay()
{
    QDateTime lateNight(QDate(2026, 7, 8), QTime(2, 15));
    QCOMPARE(businessDay(lateNight), QDate(2026, 7, 7));

    QDateTime midnight(QDate(2026, 7, 8), QTime(0, 0));
    QCOMPARE(businessDay(midnight), QDate(2026, 7, 7));
}

void TestBusinessDay::cutoffBoundary()
{
    // 05:59 still belongs to the previous day; 06:00 opens the new one.
    QCOMPARE(businessDay(QDateTime(QDate(2026, 7, 8), QTime(5, 59))), QDate(2026, 7, 7));
    QCOMPARE(businessDay(QDateTime(QDate(2026, 7, 8), QTime(6, 0))), QDate(2026, 7, 8));
}

void TestBusinessDay::customCutoffHour()
{
    QDateTime threeAm(QDate(2026, 7, 8), QTime(3, 0));
    QCOMPARE(businessDay(threeAm, 2), QDate(2026, 7, 8));
    QCOMPARE(businessDay(threeAm, 4), QDate(2026, 7, 7));
}

QTEST_APPLESS_MAIN(TestBusinessDay)
#include "tst_businessday.moc"
