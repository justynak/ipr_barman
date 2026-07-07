#include <QtTest>

#include "productmanager.h"
#include "fakebarrepository.h"

class TestProductManager : public QObject
{
    Q_OBJECT

private:
    FakeBarRepository* repo;
    ProductManager* manager;

private slots:
    void init();
    void cleanup();

    void listsCategoriesFromRepository();
    void listsProductsOfCategoryWithDerivedAvailability();
    void productByNameReturnsMatch();
    void productByNameReturnsNullForUnknown();
    void selectionHoldsProductAndQuantity();
    void selectedQuantityNeverGoesNegative();
    void clearSelectionResetsEverything();
};

void TestProductManager::init()
{
    repo = new FakeBarRepository();

    repo->AddIngredient(1, 700);  // rum: 4 cl per drink -> 175
    repo->AddIngredient(2, 50);   // lime: 1 per drink   -> 50

    QList<FakeBarRepository::RecipeLine> mojitoRecipe;
    mojitoRecipe.append(FakeBarRepository::RecipeLine(1, 4));
    mojitoRecipe.append(FakeBarRepository::RecipeLine(2, 1));
    repo->AddProduct(Product(1, "Mojito", 2200, "Cocktails", 0), mojitoRecipe);

    QList<FakeBarRepository::RecipeLine> shotRecipe;
    shotRecipe.append(FakeBarRepository::RecipeLine(1, 4));
    repo->AddProduct(Product(2, "Rum shot", 1200, "Shots", 0), shotRecipe);

    manager = new ProductManager(repo);
}

void TestProductManager::cleanup()
{
    delete manager;
    delete repo;
}

void TestProductManager::listsCategoriesFromRepository()
{
    QList<QString> categories = manager->GetCategoryList();
    QCOMPARE(categories.size(), 2);
    QVERIFY(categories.contains("Cocktails"));
    QVERIFY(categories.contains("Shots"));
}

void TestProductManager::listsProductsOfCategoryWithDerivedAvailability()
{
    QList<Product> products = manager->GetAvailableProducts("Cocktails");
    QCOMPARE(products.size(), 1);
    QCOMPARE(products.first().GetName(), QString("Mojito"));
    QCOMPARE(products.first().GetAvailable(), 50);   // limited by limes
}

void TestProductManager::productByNameReturnsMatch()
{
    manager->GetAvailableProducts("Shots");

    Product* p = manager->GetProductByName("Rum shot");
    QVERIFY(p != NULL);
    QCOMPARE(p->GetPrice(), Money(1200));
}

void TestProductManager::productByNameReturnsNullForUnknown()
{
    manager->GetAvailableProducts("Shots");

    QVERIFY(manager->GetProductByName("Absinthe") == NULL);
}

void TestProductManager::selectionHoldsProductAndQuantity()
{
    manager->SetSelectedProduct(Product(1, "Mojito", 2200, "Cocktails", 50));
    manager->SetSelectedQuantity(3);

    QCOMPARE(manager->GetSelectedProduct().GetId(), 1);
    QCOMPARE(manager->GetSelectedQuantity(), 3);
}

void TestProductManager::selectedQuantityNeverGoesNegative()
{
    manager->SetSelectedProduct(Product(1, "Mojito", 2200, "Cocktails", 50));
    manager->SetSelectedQuantity(-4);

    QCOMPARE(manager->GetSelectedQuantity(), 0);
}

void TestProductManager::clearSelectionResetsEverything()
{
    manager->SetSelectedProduct(Product(1, "Mojito", 2200, "Cocktails", 50));
    manager->SetSelectedQuantity(3);

    manager->ClearSelection();

    QVERIFY(!manager->GetSelectedProduct().IsValid());
    QCOMPARE(manager->GetSelectedQuantity(), 0);
}

QTEST_APPLESS_MAIN(TestProductManager)
#include "tst_productmanager.moc"
