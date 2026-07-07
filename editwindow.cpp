#include "editwindow.h"
#include "ui_editwindow.h"
#include "manageproductwindow.h"
#include "detailswindow.h"
#include "money.h"

constexpr int COL_PRODUCT    = 0;
constexpr int COL_ADD_ONE    = 1;
constexpr int COL_NUMBER     = 2;
constexpr int COL_REMOVE_ONE = 3;
constexpr int COL_PRICE      = 4;
constexpr int COL_DELETE     = 5;


EditWindow::EditWindow(Bartender *bartender, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditWindow),
    _bartender(bartender)
{
    ui->setupUi(this);

    ui->label_logged_as->setText(tr("Zmiana #%1 — %2 %3 (%4)")
                                 .arg(_bartender->GetShiftId())
                                 .arg(_bartender->GetName())
                                 .arg(_bartender->GetSurname())
                                 .arg(_bartender->GetRole()));

    refreshBillsCombo();
    setupProductTable();

    connect(ui->box_bills, &QComboBox::textActivated, this, &EditWindow::onBillSelected);
    connect(ui->product_list, &QTableWidget::cellClicked, this, &EditWindow::onProductCellClicked);
    connect(ui->button_add_product, &QPushButton::clicked, this, &EditWindow::onAddProductClicked);
    connect(ui->button_delete_bill, &QPushButton::clicked, this, &EditWindow::onDeleteBillClicked);
    connect(ui->button_print_bill, &QPushButton::clicked, this, &EditWindow::onPrintBillClicked);
    connect(ui->button_scan_client_card, &QPushButton::clicked, this, &EditWindow::onScanClientCardClicked);
}

EditWindow::~EditWindow()
{
    delete ui;
}

QString EditWindow::formatAmount(Money amount) const
{
    return QString("%1 %2").arg(moneyToDecimalString(amount)).arg(_bartender->GetCurrency());
}

void EditWindow::setupProductTable()
{
    ui->product_list->clear();
    ui->product_list->setRowCount(0);
    ui->product_list->setColumnCount(6);
    ui->product_list->setColumnWidth(COL_PRODUCT, 352);
    ui->product_list->setColumnWidth(COL_ADD_ONE, 50);
    ui->product_list->setColumnWidth(COL_REMOVE_ONE, 50);
    ui->product_list->setColumnWidth(COL_DELETE, 150);

    QStringList headers;
    headers << tr("Produkt") << "" << tr("Ilość") << "" << tr("Cena") << tr("Usuń");
    ui->product_list->setHorizontalHeaderLabels(headers);
}

void EditWindow::refreshBill()
{
    setupProductTable();

    QList<OrderLine> lines = _bartender->GetOrderLines();

    for(int i = 0; i < lines.count(); ++i)
    {
        QTableWidgetItem* item[6];
        for(int k = 0; k < 6; ++k)
        {
            item[k] = new QTableWidgetItem();
            item[k]->setFlags(item[k]->flags() ^ Qt::ItemIsEditable);
        }

        ui->product_list->insertRow(i);
        item[COL_PRODUCT]->setText(lines[i].productName);
        item[COL_ADD_ONE]->setText(tr("+"));
        item[COL_NUMBER]->setText(tr("%1").arg(lines[i].quantity));
        item[COL_REMOVE_ONE]->setText(tr("-"));
        item[COL_PRICE]->setText(moneyToDecimalString(lines[i].unitPrice));
        item[COL_DELETE]->setText(tr("X"));

        for(int j = 0; j < 6; ++j)
            ui->product_list->setItem(i, j, item[j]);
    }

    updateSummaryLabels();
}

void EditWindow::refreshBillsCombo()
{
    ui->box_bills->clear();
    ui->box_bills->addItem(tr("Nowy rachunek"));
    const QList<QString> labels = _bartender->GetOrders();
    for(const QString& label : labels)
        ui->box_bills->addItem(label);
}

void EditWindow::updateSummaryLabels()
{
    ui->label_bill_number->setText(tr("Numer rachunku: %1").arg(_bartender->GetSelectedOrderNumber()));
    ui->label_client_number->setText(tr("Numer klienta: %1").arg(_bartender->GetSelectedOrderCustomerID()));
    ui->label_value->setText(formatAmount(_bartender->GetSelectedOrderCost()));
}

void EditWindow::onBillSelected(const QString &label)
{
    if(label == tr("Nowy rachunek"))
    {
        _bartender->AddOrder();
        ui->box_bills->addItem(_bartender->GetSelectedOrderNumber());
    }
    else
    {
        _bartender->SetOrder(label);
    }

    refreshBill();
}

void EditWindow::onProductCellClicked(int row, int col)
{
    QList<OrderLine> lines = _bartender->GetOrderLines();
    if(row < 0 || row >= lines.size())
        return;

    OrderLine line = lines[row];

    switch(col)
    {
        case COL_ADD_ONE:
            _bartender->ChangeProductQuantity(line.productId, line.quantity + 1);
        break;

        case COL_REMOVE_ONE:
            if(line.quantity - 1 >= 1)
                _bartender->ChangeProductQuantity(line.productId, line.quantity - 1);
        break;

        case COL_DELETE:
            _bartender->RemoveProduct(line.productId);
        break;

        default:
            return;
    }

    refreshBill();
}

void EditWindow::onAddProductClicked()
{
    ManageProductWindow mpw(_bartender->GetProductManager());

    // The product is added only when the dialog was accepted.
    if(mpw.exec() == QDialog::Accepted)
    {
        _bartender->AddProduct();
        refreshBill();
    }
}

void EditWindow::onDeleteBillClicked()
{
    _bartender->RemoveOrder();

    refreshBillsCombo();
    refreshBill();
}

void EditWindow::onPrintBillClicked()
{
    DetailsWindow dw(_bartender->GetSelectedDraft(), _bartender->GetCurrency());

    if(dw.exec() == QDialog::Accepted)
    {
        // One repository call, one transaction; the draft leaves memory.
        _bartender->FinalizeOrder();

        refreshBillsCombo();
        refreshBill();
    }
}

void EditWindow::onScanClientCardClicked()
{
    _bartender->ScanCustomer();
    updateSummaryLabels();
}
