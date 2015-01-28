#include "manageproductwindow.h"
#include "ui_manageproductwindow.h"

ManageProductWindow::ManageProductWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManageProductWindow)
{
    ui->setupUi(this);
}

ManageProductWindow::~ManageProductWindow()
{
    delete ui;
}
