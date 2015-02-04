#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"
#include "editwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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
    _loginWindow = new LoginWindow();
    connect(_loginWindow, &LoginWindow::logged, this, &MainWindow::SetEditWindow);
    this->setCentralWidget(_loginWindow);
}

void MainWindow::SetEditWindow(QString b)
{
    _bartender = new Bartender(b);
    _editWindow = new EditWindow(_bartender);
    this->setCentralWidget(_editWindow);
}
