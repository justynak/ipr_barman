#ifndef MANAGEPRODUCTWINDOW_H
#define MANAGEPRODUCTWINDOW_H

#include <QWidget>
#include "productmanager.h"
#include <QDialog>

namespace Ui {
class ManageProductWindow;
}

class ManageProductWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ManageProductWindow( ProductManager* manager, QWidget *parent = 0);
    ~ManageProductWindow();

private:
    Ui::ManageProductWindow *ui;
    ProductManager* _productManager;
    Product* productSelected;

signals:
    void managerClosed();

private slots:
    void on_box_categories_activated(const QString &arg1);
    void on_box_products_activated(const QString &arg1);
    void on_button_add_clicked();
    void on_button_remove_clicked();
    void on_button_approve_clicked();
    void on_button_discard_clicked();
};

#endif // MANAGEPRODUCTWINDOW_H
