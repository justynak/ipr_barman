#include <QtTest>

#include "ordermanager.h"
#include "businessday.h"
#include "fakebarrepository.h"
#include "scriptedcardscanner.h"

// Finalize is the only DB write path: these tests assert the exact rows it
// composes — order (with copies), items (with copies) and ledger movements.
class TestFinalize : public QObject
{
    Q_OBJECT

private:
    FakeBarRepository* repo;
    ScriptedCardScanner* scanner;
    OrderManager* manager;

    void addToDraft(const Product& p, int quantity)
    {
        manager->GetProductManager()->SetSelectedProduct(p);
        manager->GetProductManager()->SetSelectedQuantity(quantity);
        manager->AddProduct();
    }

private slots:
    void init();
    void cleanup();

    void finalizeComposesOrderItemsAndMovements();
    void finalizeCopiesDiscountAndCustomer();
    void finalizeRemovesTheDraftFromMemory();
    void finalizingAnEmptyDraftFails();
    void deletingADraftTouchesNoStorage();
};

void TestFinalize::init()
{
    repo = new FakeBarRepository();
    repo->AddCustomer(5, "1001");

    // Mojito: 4 cl rum (id 1) + 1 lime (id 2); Cuba Libre: 4 cl rum + 20 cl cola (id 3)
    repo->AddIngredient(1, 700);
    repo->AddIngredient(2, 50);
    repo->AddIngredient(3, 700);

    QList<FakeBarRepository::RecipeLine> mojitoRecipe;
    mojitoRecipe.append(FakeBarRepository::RecipeLine(1, 4));
    mojitoRecipe.append(FakeBarRepository::RecipeLine(2, 1));
    repo->AddProduct(Product(1, "Mojito", 2200, "Cocktails", 0), mojitoRecipe);

    QList<FakeBarRepository::RecipeLine> cubaRecipe;
    cubaRecipe.append(FakeBarRepository::RecipeLine(1, 4));
    cubaRecipe.append(FakeBarRepository::RecipeLine(3, 20));
    repo->AddProduct(Product(2, "Cuba Libre", 2000, "Cocktails", 0), cubaRecipe);

    scanner = new ScriptedCardScanner();
    manager = new OrderManager(repo, scanner);
    manager->CreateOrder();
}

void TestFinalize::cleanup()
{
    delete manager;
    delete scanner;
    delete repo;
}

void TestFinalize::finalizeComposesOrderItemsAndMovements()
{
    addToDraft(Product(1, "Mojito", 2200, "Cocktails", 0), 2);
    addToDraft(Product(2, "Cuba Libre", 2000, "Cocktails", 0), 1);

    QVERIFY(manager->FinalizeOrder(7));

    QCOMPARE(repo->finalizedOrders.size(), 1);
    const FakeBarRepository::FinalizedOrder& order = repo->finalizedOrders.first();

    QCOMPARE(order.shiftId, 7);
    QCOMPARE(order.businessDay, businessDay(QDateTime::currentDateTime()));
    QCOMPARE(order.currency, QString("PLN"));
    QCOMPARE(order.customerId, 0);
    QCOMPARE(order.discountRate, 0.0);

    QCOMPARE(order.items.size(), 2);
    QCOMPARE(order.items[0].productName, QString("Mojito"));
    QCOMPARE(order.items[0].unitPrice, Money(2200));
    QCOMPARE(order.items[0].quantity, 2);
    QCOMPARE(order.items[1].productName, QString("Cuba Libre"));

    // Movements: mojito line -> rum -8 (4x2), lime -2 (1x2);
    // cuba line -> rum -4, cola -20. All reason 'order', linked to their item.
    QCOMPARE(repo->movements.size(), 4);

    QCOMPARE(repo->movements[0].ingredientId, 1);
    QCOMPARE(repo->movements[0].amount, -8.0);
    QCOMPARE(repo->movements[0].reason, QString("order"));
    QCOMPARE(repo->movements[0].orderItemId, order.items[0].id);

    QCOMPARE(repo->movements[1].ingredientId, 2);
    QCOMPARE(repo->movements[1].amount, -2.0);

    QCOMPARE(repo->movements[2].ingredientId, 1);
    QCOMPARE(repo->movements[2].amount, -4.0);
    QCOMPARE(repo->movements[2].orderItemId, order.items[1].id);

    QCOMPARE(repo->movements[3].ingredientId, 3);
    QCOMPARE(repo->movements[3].amount, -20.0);

    // Ledger-derived stock reflects the pours.
    QCOMPARE(repo->CurrentStock(1), 700.0 - 12.0);
    QCOMPARE(repo->CurrentStock(2), 50.0 - 2.0);
    QCOMPARE(repo->CurrentStock(3), 700.0 - 20.0);
}

void TestFinalize::finalizeCopiesDiscountAndCustomer()
{
    addToDraft(Product(1, "Mojito", 2200, "Cocktails", 0), 1);
    scanner->SetCard("1001");
    QVERIFY(manager->ScanCustomer());

    QVERIFY(manager->FinalizeOrder(7));

    const FakeBarRepository::FinalizedOrder& order = repo->finalizedOrders.first();
    QCOMPARE(order.customerId, 5);
    QCOMPARE(order.discountRate, LOYAL_CUSTOMER_DISCOUNT);
}

void TestFinalize::finalizeRemovesTheDraftFromMemory()
{
    addToDraft(Product(1, "Mojito", 2200, "Cocktails", 0), 1);

    QVERIFY(manager->FinalizeOrder(7));

    QCOMPARE(manager->GetOrders().size(), 0);
    QVERIFY(!manager->HasSelectedOrder());
    QVERIFY(!manager->FinalizeOrder(7));    // nothing selected any more
}

void TestFinalize::finalizingAnEmptyDraftFails()
{
    QVERIFY(!manager->FinalizeOrder(7));
    QCOMPARE(repo->finalizedOrders.size(), 0);
}

void TestFinalize::deletingADraftTouchesNoStorage()
{
    addToDraft(Product(1, "Mojito", 2200, "Cocktails", 0), 3);

    QVERIFY(manager->DeleteOrder());

    QCOMPARE(repo->finalizedOrders.size(), 0);
    QCOMPARE(repo->movements.size(), 0);
    QCOMPARE(manager->GetOrders().size(), 0);
}

QTEST_APPLESS_MAIN(TestFinalize)
#include "tst_finalize.moc"
