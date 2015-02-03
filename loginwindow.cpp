#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    //connect(this->ui->buttonLogin, &QPushButton::clicked, this, &LoginWindow::on_buttonLogin_clicked);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_buttonLogin_clicked()
{
    if(_loginScanner.ScanCard())
        emit logged(_loginScanner.GetCardNumber());
}
