#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include "loginscanner.h"
#include "bartender.h"


namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

signals:
    void logged(QString bartenderNumber);

private slots:
    void on_buttonLogin_clicked();

private:
    Ui::LoginWindow *ui;
    LoginScanner _loginScanner;

};

#endif // LOGINWINDOW_H
