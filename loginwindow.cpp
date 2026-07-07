#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(CardScanner* loginScanner, BarRepository* repository, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow),
    _loginScanner(loginScanner),
    _repository(repository)
{
    ui->setupUi(this);

    connect(ui->buttonLogin, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::onLoginClicked()
{
    QString cardNumber = _loginScanner->ScanCard();

    if(cardNumber != "" && _repository->FindEmployeeByCard(cardNumber).IsValid())
        emit logged(cardNumber);
}
