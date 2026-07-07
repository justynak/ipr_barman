#include "manageproductwindow.h"
#include "ui_manageproductwindow.h"

ManageProductWindow::ManageProductWindow(ProductManager *manager, QWidget *parent) :
    QDialog(parent), _productManager(manager),
    ui(new Ui::ManageProductWindow)
{
    ui->setupUi(this);

    _productManager->ClearSelection();

    //fill the box with categories
    QList<QString> categories = _productManager->GetCategoryList();

    foreach(QString cat, categories)
    {
        ui->box_categories->addItem(cat);
    }

    connect(ui->box_categories, SIGNAL(activated(QString)), this, SLOT(on_box_categories_activated(QString)));
    connect(ui->box_products, SIGNAL(activated(QString)), this, SLOT(on_box_products_activated(QString)));

}

ManageProductWindow::~ManageProductWindow()
{
    delete ui;
}

void ManageProductWindow::on_box_categories_activated(const QString &arg1)
{
    QList<Product> list = _productManager->GetAvailableProducts(arg1);
    ui->box_products->clear();

    foreach (Product p, list)
    {
        ui->box_products->addItem(p.GetName());
    }
}

void ManageProductWindow::on_box_products_activated(const QString &arg1)
{
    Product* pr =  _productManager->GetProductByName(arg1);
    if(pr == NULL) return;

    _productManager->SetSelectedProduct(*pr);

    ui->label_available->setText(tr("%1").arg(pr->GetAvailable()));
    ui->label_number->setText(tr("%1").arg(_productManager->GetSelectedQuantity()));
}

void ManageProductWindow::on_button_add_clicked()
{
    int number = _productManager->GetSelectedQuantity() + 1;

    int max = _productManager->GetSelectedProduct().GetAvailable();
    if (number > max) number = max;

    _productManager->SetSelectedQuantity(number);
    ui->label_number->setText(tr("%1").arg(_productManager->GetSelectedQuantity()));
}

void ManageProductWindow::on_button_remove_clicked()
{
    int number = _productManager->GetSelectedQuantity() - 1;
    if(number < 0) number = 0;

    _productManager->SetSelectedQuantity(number);
    ui->label_number->setText(tr("%1").arg(number));
}

void ManageProductWindow::on_button_approve_clicked()
{
    //close window
    this->done(0);
}

void ManageProductWindow::on_button_discard_clicked()
{
    // nothing selected = nothing gets added to the bill
    _productManager->ClearSelection();
    this->done(0);
}
