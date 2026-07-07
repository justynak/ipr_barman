#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(CardScanner* loginScanner, BarRepository* repository, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow),
    _loginScanner(loginScanner),
    _repository(repository)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_buttonLogin_clicked()
{
    QString cardNumber = _loginScanner->ScanCard();

    if(cardNumber != "" && _repository->FindEmployeeByCard(cardNumber).IsValid())
        emit logged(cardNumber);
}
