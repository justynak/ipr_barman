#ifndef PRODUCTMANAGER_H
#define PRODUCTMANAGER_H
#include "categorylist.h"
#include "product.h"
#include "barrepository.h"

class ProductManager
{

private:
    CategoryList* _categoryList;
    Product _productSelected;
    int _selectedQuantity;
    QList<Product> _availableProducts;
    BarRepository *db;

public:
    explicit ProductManager(BarRepository* repository);
    ~ProductManager();

    QList<QString> GetCategoryList();
    QList<Product> GetAvailableProducts(QString category);
    Product* GetProductByName(QString name);

    // Selection for "add to order": the product plus the quantity the
    // bartender dialed in the dialog.
    void SetSelectedProduct(const Product& p) { _productSelected = p; _selectedQuantity = 0; }
    Product GetSelectedProduct() { return _productSelected; }
    void SetSelectedQuantity(int quantity) { _selectedQuantity = quantity < 0 ? 0 : quantity; }
    int GetSelectedQuantity() { return _selectedQuantity; }
    void ClearSelection() { _productSelected = Product(); _selectedQuantity = 0; }

};

#endif // PRODUCTMANAGER_H
