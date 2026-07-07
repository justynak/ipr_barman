#include "detailswindow.h"
#include "ui_detailswindow.h"

DetailsWindow::DetailsWindow(DraftOrder draft, QString currency, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetailsWindow),
    _draft(draft),
    _currency(currency)
{
    ui->setupUi(this);

    //wyświetlanie szczegółów rachunku

    ui->table_costs->setRowCount(0);
    ui->table_costs->setColumnCount(3);
    ui->table_costs->setColumnWidth(0, 152);
    ui->table_costs->setColumnWidth(1, 50);
    ui->table_costs->setColumnWidth(2, 50);
    QStringList headers;
    headers << tr("Produkt") << tr("Ilość") << tr("Cena");
    ui->table_costs->setHorizontalHeaderLabels(headers);


    QList<OrderLine> lines = _draft.GetLines();

    for(int i = 0; i < lines.count(); ++i)
    {
        QTableWidgetItem* item[3];
        for(int k = 0; k < 3; ++k)
        {
            item[k] = new QTableWidgetItem();
            item[k]->setFlags(item[k]->flags() ^ Qt::ItemIsEditable);
        }

        ui->table_costs->insertRow(i);
        item[0]->setText(lines[i].productName);
        item[1]->setText(tr("%1").arg(lines[i].quantity));
        item[2]->setText(moneyToDecimalString(lines[i].unitPrice));

        for(int j = 0; j < 3; ++j)
            ui->table_costs->setItem(i, j, item[j]);
    }

    ui->label_bill_number->setText(tr("Numer rachunku: %1").arg(_draft.GetLabel()));
    ui->label_value->setText(QString("%1 %2").arg(moneyToDecimalString(_draft.Subtotal())).arg(_currency));
    ui->labelDiscount->setText(QString("%1 %2").arg(moneyToDecimalString(_draft.Total())).arg(_currency));

    connect(ui->button_ok, &QPushButton::clicked, this, &DetailsWindow::accept);
    connect(ui->button_discard, &QPushButton::clicked, this, &DetailsWindow::reject);
}

DetailsWindow::~DetailsWindow()
{
    delete ui;
}
