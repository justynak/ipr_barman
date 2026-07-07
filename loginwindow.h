#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include "cardscanner.h"
#include "barrepository.h"


namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    LoginWindow(CardScanner* loginScanner, BarRepository* repository, QWidget *parent = 0);
    ~LoginWindow();

signals:
    void logged(QString bartenderNumber);

private slots:
    void onLoginClicked();

private:
    Ui::LoginWindow *ui;
    CardScanner* _loginScanner;
    BarRepository* _repository;

};

#endif // LOGINWINDOW_H
