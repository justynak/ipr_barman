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

    Product mojito() { return Product(1, "Mojito", 2200, "Cocktails", 10); }

    void addToDraft(const Product& p, int quantity)
    {
        manager->GetProductManager()->SetSelectedProduct(p);
        manager->GetProductManager()->SetSelectedQuantity(quantity);
        manager->AddProduct();
    }

private slots:
    void init();
    void cleanup();

    void startsWithNoDrafts();
    void createOrderSelectsANewDraft();
    void draftsGetDistinctLabels();
    void selectDraftByLabel();
    void selectUnknownLabelFails();
    void deleteDraftIsMemoryOnly();
    void addProductRequiresSelectionAndQuantity();
    void changeQuantityAndCost();
    void deleteProductLine();
    void scanKnownCustomerCopiesDiscount();
    void scanUnknownCustomerLeavesDraftUntouched();
    void scanWithoutCardFails();
};

void TestOrderManager::init()
{
    repo = new FakeBarRepository();
    repo->AddCustomer(5, "1001");

    scanner = new ScriptedCardScanner();
    manager = new OrderManager(repo, scanner);
}

void TestOrderManager::cleanup()
{
    delete manager;
    delete scanner;
    delete repo;
}

void TestOrderManager::startsWithNoDrafts()
{
    QVERIFY(!manager->HasSelectedOrder());
    QCOMPARE(manager->GetSelectedOrderNumber(), QString(""));
    QCOMPARE(manager->GetCustomerID(), QString(""));
    QCOMPARE(manager->GetOrders().size(), 0);
    QCOMPARE(manager->GetCost(), Money(0));

    // nothing to mutate: everything degrades gracefully
    QVERIFY(!manager->AddProduct());
    QVERIFY(!manager->DeleteOrder());
    QVERIFY(!manager->ScanCustomer());
    QVERIFY(!manager->FinalizeOrder(1));
}

void TestOrderManager::createOrderSelectsANewDraft()
{
    QVERIFY(manager->CreateOrder());

    QVERIFY(manager->HasSelectedOrder());
    QCOMPARE(manager->GetOrders().size(), 1);
    QCOMPARE(manager->GetSelectedOrderNumber(), manager->GetOrders().first());
}

void TestOrderManager::draftsGetDistinctLabels()
{
    manager->CreateOrder();
    manager->CreateOrder();
    manager->CreateOrder();

    QList<QString> labels = manager->GetOrders();
    QCOMPARE(labels.size(), 3);
    QVERIFY(labels[0] != labels[1]);
    QVERIFY(labels[1] != labels[2]);
}

void TestOrderManager::selectDraftByLabel()
{
    manager->CreateOrder();
    QString first = manager->GetSelectedOrderNumber();
    addToDraft(mojito(), 2);

    manager->CreateOrder();
    QVERIFY(manager->GetSelectedOrderNumber() != first);

    QVERIFY(manager->SetSelectedOrder(first));
    QCOMPARE(manager->GetSelectedOrderNumber(), first);
    QCOMPARE(manager->GetSelectedOrder().GetLines().size(), 1);
}

void TestOrderManager::selectUnknownLabelFails()
{
    manager->CreateOrder();
    QString label = manager->GetSelectedOrderNumber();

    QVERIFY(!manager->SetSelectedOrder(QString("no-such-draft")));
    QCOMPARE(manager->GetSelectedOrderNumber(), label);
}

void TestOrderManager::deleteDraftIsMemoryOnly()
{
    manager->CreateOrder();
    addToDraft(mojito(), 2);

    QVERIFY(manager->DeleteOrder());

    QCOMPARE(manager->GetOrders().size(), 0);
    QVERIFY(!manager->HasSelectedOrder());
    QCOMPARE(repo->finalizedOrders.size(), 0);
    QCOMPARE(repo->movements.size(), 0);
}

void TestOrderManager::addProductRequiresSelectionAndQuantity()
{
    manager->CreateOrder();

    // no product selected
    QVERIFY(!manager->AddProduct());

    // product selected but quantity 0
    manager->GetProductManager()->SetSelectedProduct(mojito());
    QVERIFY(!manager->AddProduct());

    manager->GetProductManager()->SetSelectedQuantity(2);
    QVERIFY(manager->AddProduct());
    QCOMPARE(manager->GetSelectedOrder().GetLines().size(), 1);
    QCOMPARE(manager->GetCost(), Money(4400));
}

void TestOrderManager::changeQuantityAndCost()
{
    manager->CreateOrder();
    addToDraft(mojito(), 2);

    QVERIFY(manager->ChangeProductQuantity(1, 5));
    QCOMPARE(manager->GetCost(), Money(5 * 2200));

    QVERIFY(!manager->ChangeProductQuantity(99, 5));
}

void TestOrderManager::deleteProductLine()
{
    manager->CreateOrder();
    addToDraft(mojito(), 2);

    QVERIFY(manager->DeleteProduct(1));
    QCOMPARE(manager->GetSelectedOrder().GetLines().size(), 0);
    QCOMPARE(manager->GetCost(), Money(0));

    QVERIFY(!manager->DeleteProduct(1));
}

void TestOrderManager::scanKnownCustomerCopiesDiscount()
{
    manager->CreateOrder();

    scanner->SetCard("1001");
    QVERIFY(manager->ScanCustomer());

    QCOMPARE(manager->GetCustomerID(), QString("1001"));
    DraftOrder draft = manager->GetSelectedOrder();
    QCOMPARE(draft.GetCustomerId(), 5);
    QCOMPARE(draft.GetDiscountRate(), LOYAL_CUSTOMER_DISCOUNT);
}

void TestOrderManager::scanUnknownCustomerLeavesDraftUntouched()
{
    manager->CreateOrder();

    scanner->SetCard("stranger");
    QVERIFY(!manager->ScanCustomer());

    QCOMPARE(manager->GetCustomerID(), QString(""));
    QCOMPARE(manager->GetSelectedOrder().GetDiscountRate(), 0.0);
}

void TestOrderManager::scanWithoutCardFails()
{
    manager->CreateOrder();

    scanner->SetCard("");
    QVERIFY(!manager->ScanCustomer());
    QCOMPARE(manager->GetSelectedOrder().GetDiscountRate(), 0.0);
}

QTEST_APPLESS_MAIN(TestOrderManager)
#include "tst_ordermanager.moc"
