#include "editwindow.h"
#include "ui_editwindow.h"

EditWindow::EditWindow(Bartender *bartender, QWidget *parent) :
    QWidget(parent), _bartender(bartender),
    ui(new Ui::EditWindow)
{
    ui->setupUi(this);

    ui->label_logged_as->setText(tr("Zalogowany jako: %1 %2").arg(_bartender->GetName()).arg(_bartender->GetSurname()));

    QList<QString> list = _bartender->GetOrders();

    foreach (QString number, list)
    {
        ui->box_bills->addItem(tr("%1").arg(number));
    }

}

EditWindow::~EditWindow()
{
    delete ui;
}
