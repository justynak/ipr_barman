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
    void listsProductsOfCategory();
    void productByNameReturnsMatch();
    void productByNameReturnsNullForUnknown();
    void setProductsOnEmptyCategoryIsSafe();
    void setSelectedProductCopiesTheProduct();
};

void TestProductManager::init()
{
    repo = new FakeBarRepository();

    QList<Product> drinks;
    drinks.append(Product("Mojito", 4, 15.5));
    drinks.append(Product("Cuba Libre", 2, 18.0));
    repo->productsByCategory["Drinks"] = drinks;

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
    QCOMPARE(categories.size(), 1);
    QCOMPARE(categories.first(), QString("Drinks"));
}

void TestProductManager::listsProductsOfCategory()
{
    QList<Product> products = manager->GetAvailableProducts("Drinks");
    QCOMPARE(products.size(), 2);
    QCOMPARE(products.first().GetName(), QString("Mojito"));
}

void TestProductManager::productByNameReturnsMatch()
{
    manager->GetAvailableProducts("Drinks");

    Product* p = manager->GetProductByName("Cuba Libre");
    QVERIFY(p != NULL);
    QCOMPARE(p->GetPrice(), 18.0);
}

void TestProductManager::productByNameReturnsNullForUnknown()
{
    manager->GetAvailableProducts("Drinks");

    QVERIFY(manager->GetProductByName("Absinthe") == NULL);
}

void TestProductManager::setProductsOnEmptyCategoryIsSafe()
{
    manager->SetProducts("NoSuchCategory");

    QVERIFY(manager->GetSelectedProduct() != NULL);
    QVERIFY(manager->GetProductByName("Mojito") == NULL);
}

void TestProductManager::setSelectedProductCopiesTheProduct()
{
    Product original("Mojito", 2, 15.5);
    manager->SetSelectedProduct(&original);

    original.SetNumber(9);

    QCOMPARE(manager->GetSelectedProduct()->GetNumber(), 2u);
}

QTEST_APPLESS_MAIN(TestProductManager)
#include "tst_productmanager.moc"
