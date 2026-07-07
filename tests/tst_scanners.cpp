#include <QtTest>

#include "loginscanner.h"
#include "customerscanner.h"
#include "fakebarrepository.h"

class TestScanners : public QObject
{
    Q_OBJECT

private slots:
    void loginScannerPicksAKnownBartender();
    void loginScannerReturnsEmptyWhenNoBartenders();
    void customerScannerPicksAKnownCustomer();
    void customerScannerReturnsEmptyWhenNoCustomers();
};

void TestScanners::loginScannerPicksAKnownBartender()
{
    FakeBarRepository repo;
    repo.AddBartender("B1", "Jan", "Kowalski");
    repo.AddBartender("B2", "Anna", "Nowak");

    LoginScanner scanner(&repo);

    for(int i = 0; i < 20; ++i)
    {
        QString card = scanner.ScanCard();
        QVERIFY(repo.BartenderExists(card));
    }
}

void TestScanners::loginScannerReturnsEmptyWhenNoBartenders()
{
    FakeBarRepository repo;
    LoginScanner scanner(&repo);

    QCOMPARE(scanner.ScanCard(), QString(""));
}

void TestScanners::customerScannerPicksAKnownCustomer()
{
    FakeBarRepository repo;
    repo.customerCards << "C1" << "C2";

    CustomerScanner scanner(&repo);

    for(int i = 0; i < 20; ++i)
    {
        QString card = scanner.ScanCard();
        QVERIFY(repo.CustomerExists(card));
    }
}

void TestScanners::customerScannerReturnsEmptyWhenNoCustomers()
{
    FakeBarRepository repo;
    CustomerScanner scanner(&repo);

    QCOMPARE(scanner.ScanCard(), QString(""));
}

QTEST_APPLESS_MAIN(TestScanners)
#include "tst_scanners.moc"
