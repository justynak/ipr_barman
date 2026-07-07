#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"
#include "editwindow.h"

MainWindow::MainWindow(BarRepository* repository, CardScanner* loginScanner,
                       CardScanner* customerScanner, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _repository(repository),
    _loginScanner(loginScanner),
    _customerScanner(customerScanner),
    _bartender(NULL),
    _loginWindow(NULL),
    _editWindow(NULL)
{
    ui->setupUi(this);

    SetLoggingWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _bartender;
}

void MainWindow::SetLoggingWindow()
{
    _loginWindow = new LoginWindow(_loginScanner, _repository);
    connect(_loginWindow, &LoginWindow::logged, this, &MainWindow::SetEditWindow);
    this->setCentralWidget(_loginWindow);
}

void MainWindow::SetEditWindow(QString b)
{
    _bartender = new Bartender(_repository, _customerScanner, b);
    _editWindow = new EditWindow(_bartender);
    this->setCentralWidget(_editWindow);
}
