#include <QtTest>

#include "loginscanner.h"
#include "customerscanner.h"
#include "fakebarrepository.h"

class TestScanners : public QObject
{
    Q_OBJECT

private slots:
    void loginScannerPicksAKnownEmployee();
    void loginScannerReturnsEmptyWhenNoEmployees();
    void customerScannerPicksAKnownCustomer();
    void customerScannerReturnsEmptyWhenNoCustomers();
};

void TestScanners::loginScannerPicksAKnownEmployee()
{
    FakeBarRepository repo;
    repo.AddEmployee(1, "bartender", "card-1", "Jan", "Kowalski");
    repo.AddEmployee(2, "manager", "card-2", "Anna", "Nowak");

    LoginScanner scanner(&repo);

    for(int i = 0; i < 20; ++i)
    {
        QString card = scanner.ScanCard();
        QVERIFY(repo.FindEmployeeByCard(card).IsValid());
    }
}

void TestScanners::loginScannerReturnsEmptyWhenNoEmployees()
{
    FakeBarRepository repo;
    LoginScanner scanner(&repo);

    QCOMPARE(scanner.ScanCard(), QString(""));
}

void TestScanners::customerScannerPicksAKnownCustomer()
{
    FakeBarRepository repo;
    repo.AddCustomer(1, "C1");
    repo.AddCustomer(2, "C2");

    CustomerScanner scanner(&repo);

    for(int i = 0; i < 20; ++i)
    {
        QString card = scanner.ScanCard();
        QVERIFY(repo.FindCustomerByCard(card).IsValid());
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
