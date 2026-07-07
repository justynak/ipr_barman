#include "mainwindow.h"
#include "mysqlbarrepository.h"
#include "loginscanner.h"
#include "customerscanner.h"
#include <QApplication>
#include <QFile>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile theme(":/theme.qss");
    if(theme.open(QIODevice::ReadOnly))
        a.setStyleSheet(QString::fromUtf8(theme.readAll()));

    BarDatabaseConfig config;
    config.hostName = "localhost";
    config.databaseName = "barman_v2";
    config.userName = "bar";
    config.password = "kelner2015";
    config.barName = "Boboli 8, Warszawa";

    MySqlBarRepository repository(config);

    if(!repository.Open())
        QMessageBox::information(nullptr, QObject::tr("Error"), repository.LastError().text());

    LoginScanner loginScanner(&repository);
    CustomerScanner customerScanner(&repository);

    MainWindow w(&repository, &loginScanner, &customerScanner);
    w.show();

    return a.exec();
}
