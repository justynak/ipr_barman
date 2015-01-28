#include "detailswindow.h"
#include "ui_detailswindow.h"

DetailsWindow::DetailsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DetailsWindow)
{
    ui->setupUi(this);
}

DetailsWindow::~DetailsWindow()
{
    delete ui;
}
