#include "manageproductwindow.h"
#include "ui_manageproductwindow.h"

ManageProductWindow::ManageProductWindow(ProductManager *manager, QWidget *parent) :
    QDialog(parent), _productManager(manager),
    //    QDialog(parent),
    //ui(new Ui::Dialog)
    //
    ui(new Ui::ManageProductWindow)
{
    ui->setupUi(this);

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
    Product* p = _productManager->GetSelectedProduct();
    p->SetName(arg1);

    Product* pr =  _productManager->GetProductByName(arg1);
    int maxnumber = pr->GetNumber();
    p->SetNumber(0);

    _productManager->SetSelectedProduct(p);
    ui->label_available->setText(tr("%1").arg(maxnumber));
    ui->label_number->setText(tr("%1").arg(p->GetNumber()));
}

void ManageProductWindow::on_button_add_clicked()
{
    Product* p = _productManager->GetSelectedProduct();
    int number = p->GetNumber();
    number += 1;

    int max = ui->label_available->text().toInt();
    if (number > max) number = max;

    p->SetNumber(number);
    ui->label_number->setText(tr("%1").arg(number));
}

void ManageProductWindow::on_button_remove_clicked()
{
    Product* p = _productManager->GetSelectedProduct();
    int number = p->GetNumber();
    number -= 1;
    if(number <0) number = 0;
    p->SetNumber(number);
    ui->label_number->setText(tr("%1").arg(number));
}

void ManageProductWindow::on_button_approve_clicked()
{
    Product* p = _productManager->GetSelectedProduct();
    QString name = p->GetName();
    Product* pr =  _productManager->GetProductByName(name);

    int maxnumber = pr->GetNumber() - p->GetNumber();
    pr->SetNumber(maxnumber);

    //close window
    this->done(0);
}

void ManageProductWindow::on_button_discard_clicked()
{
    //close window
    Product* p = _productManager->GetSelectedProduct();
    p->SetName("");
    p->SetNumber(0);
    this->done(0);
}
