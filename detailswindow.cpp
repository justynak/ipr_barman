#include "detailswindow.h"
#include "ui_detailswindow.h"

DetailsWindow::DetailsWindow(DraftOrder draft, QWidget *parent) :
    QDialog(parent), _draft(draft),
    ui(new Ui::DetailsWindow)
{
    ui->setupUi(this);

    //wyświetlanie szczegółów rachunku

    ui->table_costs->setRowCount(0);
    ui->table_costs->setColumnCount(3);
    ui->table_costs->setColumnWidth(0, 152);
    ui->table_costs->setColumnWidth(1, 50);
    ui->table_costs->setColumnWidth(2, 50);
    QStringList a;
    a<<"Produkt"<<"Ilość"<<"Cena";
    ui->table_costs->setHorizontalHeaderLabels(a);


    QList<OrderLine> list = _draft.GetLines();

    for(int i=0; i<list.count(); ++i)
    {
        QTableWidgetItem* item[3];
        for(int k=0; k<3; ++k)
        {
            item[k] = new QTableWidgetItem();
            item[k]->setFlags(item[k]->flags() ^ Qt::ItemIsEditable);
        }

        OrderLine line = list.at(i);

        ui->table_costs->insertRow(i);
        item[0]->setText(line.productName);
        item[1]->setText(tr("%1").arg(line.quantity));
        item[2]->setText(moneyToDecimalString(line.unitPrice));

        for(unsigned int j=0; j<3; ++j)
        {
            ui->table_costs->setItem(i, j, item[j]);
        }
    }

    ui->label_bill_number->setText(tr("Numer rachunku: %1").arg(_draft.GetLabel()));
    ui->label_value->setText(tr("%1 zł").arg(moneyToDecimalString(_draft.Subtotal())));
    ui->labelDiscount->setText(tr("%1 zł").arg(moneyToDecimalString(_draft.Total())));

}

DetailsWindow::~DetailsWindow()
{
    delete ui;
}

void DetailsWindow::on_button_ok_clicked()
{
    this->done(1);
}

void DetailsWindow::on_button_discard_clicked()
{
    this->done(0);
}
