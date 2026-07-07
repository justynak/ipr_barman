#include <QtTest>

#include "ordermanager.h"
#include "fakebarrepository.h"
#include "scriptedcardscanner.h"

class TestOrderManager : public QObject
{
    Q_OBJECT

private:
    FakeBarRepository* repo;
    ScriptedCardScanner* scanner;
    OrderManager* manager;

    Product mojito() { return Product("Mojito", 2, 15.5); }

private slots:
    void init();
    void cleanup();

    void startsWithNoSelectedOrder();
    void listsExistingOrdersOfBartender();
    void createOrderStoresAndSelectsIt();
    void selectOrderByNameLoadsProductsAndCost();
    void selectUnknownOrderFails();
    void addProductStoresLineAndUpdatesCost();
    void changeProductNumberUpdatesRepositoryAndCost();
    void deleteProductRemovesLineAndUpdatesCost();
    void deleteOrderRemovesItFromRepository();
    void closeOrderMarksItClosed();
    void scanKnownCustomerSetsDiscountAndCustomer();
    void scanUnknownCustomerGivesNoDiscount();
    void scanWithoutCardGivesNoDiscount();
};

void TestOrderManager::init()
{
    repo = new FakeBarRepository();
    repo->AddBartender("B1", "Jan", "Kowalski");
    repo->customerCards << "C1";

    scanner = new ScriptedCardScanner();
    manager = NULL;
}

void TestOrderManager::cleanup()
{
    delete manager;
    delete scanner;
    delete repo;
}

void TestOrderManager::startsWithNoSelectedOrder()
{
    manager = new OrderManager(repo, scanner, "B1");

    QCOMPARE(manager->GetSelectedOrderNumber(), QString(""));
    QCOMPARE(manager->GetCustomerID(), QString(""));
    QCOMPARE(manager->GetOrders().size(), 0);
    QCOMPARE(manager->GetProducts().size(), 0);
    QCOMPARE(manager->GetCost(), 0.0);
}

void TestOrderManager::listsExistingOrdersOfBartender()
{
    Order o1, o2, other;
    repo->CreateOrder("B1", &o1);
    repo->CreateOrder("B1", &o2);
    repo->CreateOrder("B2", &other);

    manager = new OrderManager(repo, scanner, "B1");

    QList<QString> orders = manager->GetOrders();
    QCOMPARE(orders.size(), 2);
    QVERIFY(orders.contains(o1.GetOrderNumber()));
    QVERIFY(orders.contains(o2.GetOrderNumber()));
    QVERIFY(!orders.contains(other.GetOrderNumber()));
}

void TestOrderManager::createOrderStoresAndSelectsIt()
{
    manager = new OrderManager(repo, scanner, "B1");

    QVERIFY(manager->CreateOrder("B1"));

    QString number = manager->GetSelectedOrderNumber();
    QVERIFY(number != "");
    QVERIFY(repo->orders.contains(number));
    QCOMPARE(manager->GetOrders().size(), 1);
}

void TestOrderManager::selectOrderByNameLoadsProductsAndCost()
{
    Order o;
    repo->CreateOrder("B1", &o);
    repo->AddProductToOrder(o.GetOrderNumber(), mojito());

    manager = new OrderManager(repo, scanner, "B1");

    QVERIFY(manager->SetSelectedOrder(o.GetOrderNumber()));
    QCOMPARE(manager->GetSelectedOrderNumber(), o.GetOrderNumber());
    QCOMPARE(manager->GetProducts().size(), 1);
    QCOMPARE(manager->GetCost(), 2 * 15.5);
}

void TestOrderManager::selectUnknownOrderFails()
{
    manager = new OrderManager(repo, scanner, "B1");

    QVERIFY(!manager->SetSelectedOrder(QString("no-such-bill")));
    QCOMPARE(manager->GetSelectedOrderNumber(), QString(""));
}

void TestOrderManager::addProductStoresLineAndUpdatesCost()
{
    manager = new OrderManager(repo, scanner, "B1");
    manager->CreateOrder("B1");

    Product p = mojito();
    manager->SetSelectedProduct(&p);
    QVERIFY(manager->AddProduct());

    QString number = manager->GetSelectedOrderNumber();
    QCOMPARE(repo->orders[number].items.size(), 1);
    QCOMPARE(repo->orders[number].items.first().GetName(), QString("Mojito"));
    QCOMPARE(manager->GetProducts().size(), 1);
    QCOMPARE(manager->GetCost(), 2 * 15.5);
}

void TestOrderManager::changeProductNumberUpdatesRepositoryAndCost()
{
    manager = new OrderManager(repo, scanner, "B1");
    manager->CreateOrder("B1");

    Product p = mojito();
    manager->SetSelectedProduct(&p);
    manager->AddProduct();

    QVERIFY(manager->ChangeProductNumber(&p, 5));

    QString number = manager->GetSelectedOrderNumber();
    QCOMPARE(repo->orders[number].items.first().GetNumber(), 5u);
    QCOMPARE(manager->GetCost(), 5 * 15.5);
}

void TestOrderManager::deleteProductRemovesLineAndUpdatesCost()
{
    manager = new OrderManager(repo, scanner, "B1");
    manager->CreateOrder("B1");

    Product p = mojito();
    manager->SetSelectedProduct(&p);
    manager->AddProduct();

    QVERIFY(manager->DeleteProduct(&p));

    QString number = manager->GetSelectedOrderNumber();
    QCOMPARE(repo->orders[number].items.size(), 0);
    QCOMPARE(manager->GetProducts().size(), 0);
    QCOMPARE(manager->GetCost(), 0.0);
}

void TestOrderManager::deleteOrderRemovesItFromRepository()
{
    manager = new OrderManager(repo, scanner, "B1");
    manager->CreateOrder("B1");
    QString number = manager->GetSelectedOrderNumber();

    Product p = mojito();
    manager->SetSelectedProduct(&p);
    manager->AddProduct();

    QVERIFY(manager->DeleteOrder());

    QVERIFY(!repo->orders.contains(number));
}

void TestOrderManager::closeOrderMarksItClosed()
{
    manager = new OrderManager(repo, scanner, "B1");
    manager->CreateOrder("B1");
    QString number = manager->GetSelectedOrderNumber();

    QVERIFY(manager->CloseOrder());

    QVERIFY(repo->orders[number].closed);
}

void TestOrderManager::scanKnownCustomerSetsDiscountAndCustomer()
{
    manager = new OrderManager(repo, scanner, "B1");
    manager->CreateOrder("B1");
    QString number = manager->GetSelectedOrderNumber();

    scanner->SetCard("C1");
    QVERIFY(manager->ScanCustomer());

    QCOMPARE(manager->GetCustomerID(), QString("C1"));
    QCOMPARE(repo->orders[number].customerID, QString("C1"));
    QCOMPARE(manager->GetOrderDetails()->GetDiscount(), LOYAL_CUSTOMER_DISCOUNT);
}

void TestOrderManager::scanUnknownCustomerGivesNoDiscount()
{
    manager = new OrderManager(repo, scanner, "B1");
    manager->CreateOrder("B1");
    QString number = manager->GetSelectedOrderNumber();

    scanner->SetCard("stranger");
    QVERIFY(!manager->ScanCustomer());

    QCOMPARE(manager->GetCustomerID(), QString(""));
    QCOMPARE(repo->orders[number].customerID, QString(""));
    QCOMPARE(manager->GetOrderDetails()->GetDiscount(), 0.0);
}

void TestOrderManager::scanWithoutCardGivesNoDiscount()
{
    manager = new OrderManager(repo, scanner, "B1");
    manager->CreateOrder("B1");

    scanner->SetCard("");
    QVERIFY(!manager->ScanCustomer());

    QCOMPARE(manager->GetOrderDetails()->GetDiscount(), 0.0);
}

QTEST_APPLESS_MAIN(TestOrderManager)
#include "tst_ordermanager.moc"
