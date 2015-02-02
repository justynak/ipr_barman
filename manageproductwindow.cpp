#include "manageproductwindow.h"
#include "ui_manageproductwindow.h"

ManageProductWindow::ManageProductWindow(ProductManager *manager, QWidget *parent) :
    QWidget(parent), _productManager(manager),
    ui(new Ui::ManageProductWindow)
{
    ui->setupUi(this);

    //fill the box with categories
    QList<QString> categories = _productManager->GetCategoryList();

    foreach(QString cat, categories)
    {
        ui->box_categories->addItem(cat);
    }
}

ManageProductWindow::~ManageProductWindow()
{
    delete ui;
}

void ManageProductWindow::on_box_categories_activated(const QString &arg1)
{

}

void ManageProductWindow::on_box_products_activated(const QString &arg1)
{

}

void ManageProductWindow::on_button_add_clicked()
{

}

void ManageProductWindow::on_button_remove_clicked()
{

}

void ManageProductWindow::on_button_approve_clicked()
{

}

void ManageProductWindow::on_button_discard_clicked()
{

}
