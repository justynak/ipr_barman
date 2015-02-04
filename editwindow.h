#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <QWidget>
#include "detailswindow.h"
#include "bartender.h"


namespace Ui {
class EditWindow;
}

class EditWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EditWindow(Bartender* bartender, QWidget *parent = 0);
    ~EditWindow();

private slots:
    void on_box_bills_activated(const QString &arg1);
    void on_product_list_cellActivated(int row, int column);
    void on_button_add_product_clicked();

    void on_button_delete_bill_clicked();

    void on_button_print_bill_clicked();

    void on_button_scan_client_card_clicked();

private:
    Ui::EditWindow *ui;
    DetailsWindow* _detailsWindow;
    Bartender* _bartender;
};

#endif // EDITWINDOW_H
