#include "editwindow.h"
#include "ui_editwindow.h"
#include "manageproductwindow.h"
#include "detailswindow.h"
#include "money.h"

#define COL_PRODUCT     0
#define COL_ADD_ONE     1
#define COL_REMOVE_ONE  3
#define COL_NUMBER      2
#define COL_PRICE       4
#define COL_DELETE      5


EditWindow::EditWindow(Bartender *bartender, QWidget *parent) :
    QWidget(parent), _bartender(bartender),
    ui(new Ui::EditWindow)
{
    ui->setupUi(this);

    ui->label_logged_as->setText(tr("Zalogowany jako: %1 %2").arg(_bartender->GetName()).arg(_bartender->GetSurname()));

    QList<QString> list = _bartender->GetOrders();
    ui->box_bills->addItem(tr("Nowy rachunek"));
    foreach (QString number, list)
    {
        ui->box_bills->addItem(tr("%1").arg(number));
    }

    ui->product_list->setRowCount(0);
    ui->product_list->setColumnCount(6);
    ui->product_list->setColumnWidth(COL_PRODUCT, 352);
    ui->product_list->setColumnWidth(COL_ADD_ONE, 50);
    ui->product_list->setColumnWidth(COL_REMOVE_ONE, 50);
    ui->product_list->setColumnWidth(COL_DELETE, 150);
    QStringList a;
    a<<"Produkt"<<""<<"Ilość"<<""<<"Cena"<<"Usuń";
    ui->product_list->setHorizontalHeaderLabels(a);

    //connects
    connect(ui->box_bills, SIGNAL(activated(QString)), this, SLOT(on_box_bills_activated(QString)));
    connect(ui->product_list, SIGNAL(cellClicked(int,int)), this, SLOT(on_product_list_cellActivated(int,int)));
    connect(ui->button_delete_bill, SIGNAL(clicked()), this, SLOT(on_button_delete_bill_clicked()));
}

EditWindow::~EditWindow()
{
    delete ui;
}

void EditWindow::on_box_bills_activated(const QString &arg1)
{
    ui->product_list->clear();

    ui->product_list->setRowCount(0);
    ui->product_list->setColumnCount(6);
    ui->product_list->setColumnWidth(COL_PRODUCT, 352);
    ui->product_list->setColumnWidth(COL_ADD_ONE, 50);
    ui->product_list->setColumnWidth(COL_REMOVE_ONE, 50);
    ui->product_list->setColumnWidth(COL_DELETE, 150);
    QStringList a;
    a<<"Produkt"<<""<<"Ilość"<<""<<"Cena"<<"Usuń";
    ui->product_list->setHorizontalHeaderLabels(a);

    if(arg1 == "Nowy rachunek")
    {
       _bartender->AddOrder();

       // the new draft appears in the combo box under its label
       ui->box_bills->addItem(_bartender->GetSelectedOrderNumber());
    }

    else
    {
        _bartender->SetOrder(arg1);
    }

    ui->label_bill_number->setText(tr("Numer rachunku: %1 ").arg(_bartender->GetSelectedOrderNumber()));
    QList<OrderLine> list = _bartender->GetOrderLines();

    for(int i=0; i<list.count(); ++i)
    {
        QTableWidgetItem* item[6];
        for(int k=0; k<6; ++k)
        {
            item[k] = new QTableWidgetItem();
            item[k]->setFlags(item[k]->flags() ^ Qt::ItemIsEditable);
        }

        ui->product_list->insertRow(i);
        item[COL_PRODUCT]->setText(list[i].productName);
        item[COL_ADD_ONE]->setText(tr("+"));
        item[COL_NUMBER]->setText(tr("%1").arg(list[i].quantity));
        item[COL_REMOVE_ONE]->setText(tr("-"));
        item[COL_PRICE]->setText(moneyToDecimalString(list[i].unitPrice));
        item[COL_DELETE]->setText(tr("X"));

        for(unsigned int j=0; j<6; ++j)
        {
            ui->product_list->setItem(i, j, item[j]);
        }

    }
    ui->label_client_number->setText(tr("Numer klienta: %1").arg(_bartender->GetSelectedOrderCustomerID()));
    ui->label_value->setText(tr("%1 zł").arg(moneyToDecimalString(_bartender->GetSelectedOrderCost())));
}



void EditWindow::on_product_list_cellActivated(int row, int col)
{
    QList<OrderLine> list = _bartender->GetOrderLines();
    if(row < 0 || row >= list.size())
        return;

    OrderLine line = list[row];
    int number = line.quantity;

    switch(col)
    {
        case COL_ADD_ONE:
            number+=1;
            _bartender->ChangeProductQuantity(line.productId, number);
            ui->product_list->item(row, COL_NUMBER)->setText(tr("%1").arg(number));
        break;

        case COL_REMOVE_ONE:
            number-=1;
            if(number>=1)
            {
                _bartender->ChangeProductQuantity(line.productId, number);
                ui->product_list->item(row, COL_NUMBER)->setText(tr("%1").arg(number));
            }
        break;

        case COL_DELETE:
             _bartender->RemoveProduct(line.productId);
             ui->product_list->removeRow(row);
        break;
    }
    ui->label_client_number->setText(tr("Numer klienta: %1").arg(_bartender->GetSelectedOrderCustomerID()));
    ui->label_value->setText(tr("%1 zł").arg(moneyToDecimalString(_bartender->GetSelectedOrderCost())));
}

void EditWindow::on_button_add_product_clicked()
{
    ProductManager* manager = _bartender->GetProductManager();
    ManageProductWindow mpw(manager);

    mpw.exec();

    _bartender->AddProduct();
    ui->label_client_number->setText(tr("Numer klienta: %1").arg(_bartender->GetSelectedOrderCustomerID()));
    ui->label_value->setText(tr("%1 zł").arg(moneyToDecimalString(_bartender->GetSelectedOrderCost())));
}

void EditWindow::on_button_delete_bill_clicked()
{
    _bartender->RemoveOrder();

    //refresh gui
    ui->box_bills->clear();
    QList<QString> list = _bartender->GetOrders();
    ui->box_bills->addItem(tr("Nowy rachunek"));
    foreach (QString number, list)
    {
        ui->box_bills->addItem(tr("%1").arg(number));
    }

    ui->product_list->clear();
    ui->label_bill_number->setText(tr("Numer rachunku: "));
    ui->product_list->setRowCount(0);
    ui->product_list->setColumnCount(6);
    ui->product_list->setColumnWidth(COL_PRODUCT, 352);
    ui->product_list->setColumnWidth(COL_ADD_ONE, 50);
    ui->product_list->setColumnWidth(COL_REMOVE_ONE, 50);
    ui->product_list->setColumnWidth(COL_DELETE, 150);
    QStringList a;
    a<<"Produkt"<<""<<"Ilość"<<""<<"Cena"<<"Usuń";
    ui->product_list->setHorizontalHeaderLabels(a);

    ui->label_client_number->setText(tr("Numer klienta: "));
    ui->label_value->setText(tr(" zł"));

}

void EditWindow::on_button_print_bill_clicked()
{
    DetailsWindow dw(_bartender->GetSelectedDraft());
    int result = dw.exec();

    if(result == 1)
    {
        // One repository call, one transaction; the draft leaves memory.
        _bartender->FinalizeOrder();

        //przełącz widok
        ui->product_list->clear();
        ui->label_bill_number->setText(tr("Numer rachunku: "));
        ui->product_list->setRowCount(0);
        ui->product_list->setColumnCount(6);
        ui->product_list->setColumnWidth(COL_PRODUCT, 352);
        ui->product_list->setColumnWidth(COL_ADD_ONE, 50);
        ui->product_list->setColumnWidth(COL_REMOVE_ONE, 50);
        ui->product_list->setColumnWidth(COL_DELETE, 150);
        QStringList a;
        a<<"Produkt"<<""<<"Ilość"<<""<<"Cena"<<"Usuń";
        ui->product_list->setHorizontalHeaderLabels(a);

        //refresh gui
        ui->box_bills->clear();
        QList<QString> list = _bartender->GetOrders();
        ui->box_bills->addItem(tr("Nowy rachunek"));
        foreach (QString number, list)
        {
            ui->box_bills->addItem(tr("%1").arg(number));
        }

        ui->label_client_number->setText(tr("Numer klienta: "));
        ui->label_value->setText(tr("Koszt zamówienia:  zł"));

    }
}

void EditWindow::on_button_scan_client_card_clicked()
{
    _bartender->ScanCustomer();
    ui->label_client_number->setText(tr("Numer klienta: %1").arg(_bartender->GetSelectedOrderCustomerID()));
    ui->label_value->setText(tr("%1 zł").arg(moneyToDecimalString(_bartender->GetSelectedOrderCost())));
}
