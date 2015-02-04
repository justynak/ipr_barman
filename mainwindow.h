#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bartender.h"
#include "loginwindow.h"
#include "editwindow.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Bartender* _bartender;
    LoginWindow* _loginWindow;
    EditWindow* _editWindow;

private slots:
    void SetLoggingWindow();
    void SetEditWindow(QString b);

};

#endif // MAINWINDOW_H
