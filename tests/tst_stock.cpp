#include <QtTest>

#include "fakebarrepository.h"
#include "ordermanager.h"
#include "scriptedcardscanner.h"

// Availability is never stored: it is floor(min(stock / recipe amount)) over
// a product's ingredients, with stock summed from the movement ledger.
class TestStock : public QObject
{
    Q_OBJECT

private slots:
    void availabilityIsMinOverIngredients();
    void availabilityFloorsFractions();
    void emptyStockMeansZeroAvailability();
    void finalizedOrdersReduceAvailability();
};

void TestStock::availabilityIsMinOverIngredients()
{
    FakeBarRepository repo;
    repo.AddIngredient(1, 700);   // rum, 4 cl per drink  -> 175 possible
    repo.AddIngredient(2, 50);    // lime, 1 per drink    -> 50 possible

    QList<FakeBarRepository::RecipeLine> recipe;
    recipe.append(FakeBarRepository::RecipeLine(1, 4));
    recipe.append(FakeBarRepository::RecipeLine(2, 1));
    repo.AddProduct(Product(1, "Mojito", 2200, "Cocktails", 0), recipe);

    QList<Product> products = repo.GetProductsFromCategory("Cocktails");
    QCOMPARE(products.size(), 1);
    QCOMPARE(products.first().GetAvailable(), 50);
}

void TestStock::availabilityFloorsFractions()
{
    FakeBarRepository repo;
    repo.AddIngredient(1, 10);    // 4 cl per drink -> 2.5 -> 2

    QList<FakeBarRepository::RecipeLine> recipe;
    recipe.append(FakeBarRepository::RecipeLine(1, 4));
    repo.AddProduct(Product(1, "Rum shot", 1200, "Shots", 0), recipe);

    QCOMPARE(repo.GetProductsFromCategory("Shots").first().GetAvailable(), 2);
}

void TestStock::emptyStockMeansZeroAvailability()
{
    FakeBarRepository repo;
    repo.AddIngredient(1, 0);

    QList<FakeBarRepository::RecipeLine> recipe;
    recipe.append(FakeBarRepository::RecipeLine(1, 4));
    repo.AddProduct(Product(1, "Rum shot", 1200, "Shots", 0), recipe);

    QCOMPARE(repo.GetProductsFromCategory("Shots").first().GetAvailable(), 0);
}

void TestStock::finalizedOrdersReduceAvailability()
{
    FakeBarRepository repo;
    repo.AddIngredient(1, 40);    // 4 cl per shot -> 10 available

    QList<FakeBarRepository::RecipeLine> recipe;
    recipe.append(FakeBarRepository::RecipeLine(1, 4));
    repo.AddProduct(Product(1, "Rum shot", 1200, "Shots", 0), recipe);

    ScriptedCardScanner scanner;
    OrderManager manager(&repo, &scanner);
    manager.CreateOrder();
    manager.GetProductManager()->SetSelectedProduct(Product(1, "Rum shot", 1200, "Shots", 0));
    manager.GetProductManager()->SetSelectedQuantity(3);
    manager.AddProduct();
    QVERIFY(manager.FinalizeOrder(1));

    // 40 - 3*4 = 28 -> 7 shots left
    QCOMPARE(repo.GetProductsFromCategory("Shots").first().GetAvailable(), 7);
}

QTEST_APPLESS_MAIN(TestStock)
#include "tst_stock.moc"
