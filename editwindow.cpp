#include "editwindow.h"
#include "ui_editwindow.h"
#include "manageproductwindow.h"
#include "detailswindow.h"

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
    }

    else
    {
        _bartender->SetOrder(arg1);
    }

    ui->label_bill_number->setText(tr("Numer rachunku: %1 ").arg(_bartender->GetSelectedOrderNumber()));
    QList<Product> list = _bartender->GetProductsFromOrder();

    for(int i=0; i<list.count(); ++i)
    {
        QTableWidgetItem* item[6];
        for(int k=0; k<6; ++k)
        {
            item[k] = new QTableWidgetItem();
            item[k]->setFlags(item[k]->flags() ^ Qt::ItemIsEditable);
        }

        ui->product_list->insertRow(i);
        item[COL_PRODUCT]->setText(list[i].GetName());
        item[COL_ADD_ONE]->setText(tr("+"));
        item[COL_NUMBER]->setText(tr("%1").arg(list[i].GetNumber()));
        item[COL_REMOVE_ONE]->setText(tr("-"));
        item[COL_PRICE]->setText(tr("%1").arg(list[i].GetPrice()));
        item[COL_DELETE]->setText(tr("X"));

        for(unsigned int j=0; j<6; ++j)
        {
            ui->product_list->setItem(i, j, item[j]);
        }

    }
    ui->label_client_number->setText(tr("Numer klienta: %1").arg(_bartender->GetSelectedOrderCustomerID()));

}



void EditWindow::on_product_list_cellActivated(int row, int col)
{
    QList<Product> list = _bartender->GetProductsFromOrder();
    Product p = list[row];
    int number = p.GetNumber();

    switch(col)
    {
        case COL_ADD_ONE:
            number+=1;
            _bartender->ChangeProductNumber(p, number);
            ui->product_list->item(row, COL_NUMBER)->setText(tr("%1").arg(number));
        break;

        case COL_REMOVE_ONE:
            number-=1;
            if(number>=1)
            {
                _bartender->ChangeProductNumber(p, number);
                ui->product_list->item(row, COL_NUMBER)->setText(tr("%1").arg(number));
            }
        break;

        case COL_DELETE:
             _bartender->RemoveProduct(p);
             ui->product_list->removeRow(row);
             //delete from ui
        break;
    }
}

void EditWindow::on_button_add_product_clicked()
{
    ProductManager* manager = _bartender->GetProductManager();
    ManageProductWindow mpw(manager);

    mpw.exec();

    _bartender->AddProduct();
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

}

void EditWindow::on_button_print_bill_clicked()
{
    OrderDetails *o = _bartender->GetOrderDetails();

    /*
    connect(YesButton, SIGNAL(clicked()), dlg, SLOT(accept()));
    connect(NoButton, SIGNAL(clicked()), dlg, SLOT(reject()));
    */

    DetailsWindow dw(o);
    int result = dw.exec();

    if(result == 1)
    {
        _bartender->CloseOrder();

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

    }
}

void EditWindow::on_button_scan_client_card_clicked()
{
    _bartender->ScanCustomer();
    //ui->label_client_number->setText();
}
