#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bartender.h"
#include "barrepository.h"
#include "cardscanner.h"
#include "loginwindow.h"
#include "editwindow.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(BarRepository* repository, CardScanner* loginScanner,
               CardScanner* customerScanner, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    BarRepository* _repository;
    CardScanner* _loginScanner;
    CardScanner* _customerScanner;
    Bartender* _bartender;
    LoginWindow* _loginWindow;
    EditWindow* _editWindow;

private slots:
    void SetLoggingWindow();
    void SetEditWindow(QString b);

};

#endif // MAINWINDOW_H
