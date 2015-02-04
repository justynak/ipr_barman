#include "detailswindow.h"
#include "ui_detailswindow.h"

DetailsWindow::DetailsWindow(Order *o, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetailsWindow)
{
    ui->setupUi(this);

    _details = new OrderDetails(o);

    //wyświetlanie szczegółów rachunku

    ui->table_costs->setRowCount(0);
    ui->table_costs->setColumnCount(3);
    ui->table_costs->setColumnWidth(0, 152);
    ui->table_costs->setColumnWidth(1, 50);
    ui->table_costs->setColumnWidth(2, 50);
    QStringList a;
    a<<"Produkt"<<"Ilość"<<"Cena";
    ui->table_costs->setHorizontalHeaderLabels(a);


    QList<Product>* list = _details->GetProductList();

    for(int i=0; i<list->count(); ++i)
    {
        QTableWidgetItem* item[3];
        for(int k=0; k<3; ++k)
        {
            item[k] = new QTableWidgetItem();
            item[k]->setFlags(item[k]->flags() ^ Qt::ItemIsEditable);
        }

        Product p = list->at(i);

        ui->table_costs->insertRow(i);
        item[0]->setText(p.GetName());
        item[1]->setText(tr("%1").arg(p.GetNumber()));
        item[2]->setText(tr("%1").arg(p.GetPrice()));

        for(unsigned int j=0; j<3; ++j)
        {
            ui->table_costs->setItem(i, j, item[j]);
        }
    }

    ui->label_bill_number->setText(tr("Numer rachunku: %1").arg(_details->GetOrderNumber()));
    ui->label_value->setText(tr("%1 zł").arg(_details->GetCost()));

}

DetailsWindow::~DetailsWindow()
{
    delete ui;
    //delete _details;
}

void DetailsWindow::on_button_ok_clicked()
{
    this->done(1);
}

void DetailsWindow::on_button_discard_clicked()
{
    this->done(0);
}
