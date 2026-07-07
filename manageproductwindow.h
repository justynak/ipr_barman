#ifndef MANAGEPRODUCTWINDOW_H
#define MANAGEPRODUCTWINDOW_H

#include <QWidget>
#include "productmanager.h"
#include <QDialog>

namespace Ui {
class ManageProductWindow;
}

// Modal product picker. exec() returns QDialog::Accepted only when the
// bartender approved a choice; the chosen product and quantity are left in
// the ProductManager selection.
class ManageProductWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ManageProductWindow(ProductManager* manager, QWidget *parent = nullptr);
    ~ManageProductWindow() override;

private:
    Ui::ManageProductWindow *ui;
    ProductManager* _productManager;

private slots:
    void onCategorySelected(const QString &category);
    void onProductSelected(const QString &name);
    void onAddClicked();
    void onRemoveClicked();
};

#endif // MANAGEPRODUCTWINDOW_H
