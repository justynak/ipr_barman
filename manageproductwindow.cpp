#include "manageproductwindow.h"
#include "ui_manageproductwindow.h"

ManageProductWindow::ManageProductWindow(ProductManager *manager, QWidget *parent) :
    QDialog(parent), _productManager(manager),
    ui(new Ui::ManageProductWindow)
{
    ui->setupUi(this);

    _productManager->ClearSelection();

    //fill the box with categories
    foreach(QString category, _productManager->GetCategoryList())
    {
        ui->box_categories->addItem(category);
    }

    connect(ui->box_categories, &QComboBox::textActivated, this, &ManageProductWindow::onCategorySelected);
    connect(ui->box_products, &QComboBox::textActivated, this, &ManageProductWindow::onProductSelected);
    connect(ui->button_add, &QPushButton::clicked, this, &ManageProductWindow::onAddClicked);
    connect(ui->button_remove, &QPushButton::clicked, this, &ManageProductWindow::onRemoveClicked);
    connect(ui->button_approve, &QPushButton::clicked, this, &ManageProductWindow::accept);
    connect(ui->button_discard, &QPushButton::clicked, this, &ManageProductWindow::reject);
}

ManageProductWindow::~ManageProductWindow()
{
    delete ui;
}

void ManageProductWindow::onCategorySelected(const QString &category)
{
    ui->box_products->clear();

    foreach (Product p, _productManager->GetAvailableProducts(category))
    {
        ui->box_products->addItem(p.GetName());
    }
}

void ManageProductWindow::onProductSelected(const QString &name)
{
    Product* pr = _productManager->GetProductByName(name);
    if(pr == NULL) return;

    _productManager->SetSelectedProduct(*pr);

    ui->label_available->setText(tr("%1").arg(pr->GetAvailable()));
    ui->label_number->setText(tr("%1").arg(_productManager->GetSelectedQuantity()));
}

void ManageProductWindow::onAddClicked()
{
    int number = _productManager->GetSelectedQuantity() + 1;

    int max = _productManager->GetSelectedProduct().GetAvailable();
    if (number > max) number = max;

    _productManager->SetSelectedQuantity(number);
    ui->label_number->setText(tr("%1").arg(_productManager->GetSelectedQuantity()));
}

void ManageProductWindow::onRemoveClicked()
{
    int number = _productManager->GetSelectedQuantity() - 1;
    if(number < 0) number = 0;

    _productManager->SetSelectedQuantity(number);
    ui->label_number->setText(tr("%1").arg(number));
}
