#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"
#include "editwindow.h"

#include <QDateTime>

MainWindow::MainWindow(BarRepository* repository, CardScanner* loginScanner,
                       CardScanner* customerScanner, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _repository(repository),
    _loginScanner(loginScanner),
    _customerScanner(customerScanner)
{
    ui->setupUi(this);

    SetLoggingWindow();
}

MainWindow::~MainWindow()
{
    if(_bartender != nullptr)
        _repository->CloseShift(_bartender->GetShiftId(), QDateTime::currentDateTime());

    delete ui;
    delete _bartender;
}

void MainWindow::SetLoggingWindow()
{
    _loginWindow = new LoginWindow(_loginScanner, _repository);
    connect(_loginWindow, &LoginWindow::logged, this, &MainWindow::SetEditWindow);
    this->setCentralWidget(_loginWindow);
}

void MainWindow::SetEditWindow(QString cardNumber)
{
    Employee employee = _repository->FindEmployeeByCard(cardNumber);
    if(!employee.IsValid())
        return;

    // Logging in opens a shift; the shift closes when the app exits.
    int shiftId = _repository->OpenShift(employee.id, QDateTime::currentDateTime());

    _bartender = new Bartender(_repository, _customerScanner, employee, shiftId);
    _editWindow = new EditWindow(_bartender);
    this->setCentralWidget(_editWindow);
}
