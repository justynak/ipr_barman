#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <QWidget>
#include "bartender.h"


namespace Ui {
class EditWindow;
}

class EditWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EditWindow(Bartender* bartender, QWidget *parent = nullptr);
    ~EditWindow() override;

private slots:
    void onBillSelected(const QString &label);
    void onProductCellClicked(int row, int column);
    void onAddProductClicked();
    void onDeleteBillClicked();
    void onPrintBillClicked();
    void onScanClientCardClicked();

private:
    Ui::EditWindow *ui;
    Bartender* _bartender;

    void setupProductTable();
    void refreshBill();
    void refreshBillsCombo();
    void updateSummaryLabels();
    QString formatAmount(Money amount) const;
};

#endif // EDITWINDOW_H
