#ifndef MANAGEPRODUCTWINDOW_H
#define MANAGEPRODUCTWINDOW_H

#include <QWidget>
#include "productmanager.h"


namespace Ui {
class ManageProductWindow;
}

class ManageProductWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ManageProductWindow( ProductManager* manager, QWidget *parent = 0);
    ~ManageProductWindow();

private:
    Ui::ManageProductWindow *ui;
    ProductManager* _productManager;

signals:
    void managerClosed();
    void productSelected(Product* p);


private slots:
    void on_box_categories_activated(const QString &arg1);
    void on_box_products_activated(const QString &arg1);
    void on_button_add_clicked();
    void on_button_remove_clicked();
    void on_button_approve_clicked();
    void on_button_discard_clicked();
};

#endif // MANAGEPRODUCTWINDOW_H
