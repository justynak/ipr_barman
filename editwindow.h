#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <QWidget>
#include "detailswindow.h"
#include "manageproductwindow.h"


namespace Ui {
class EditWindow;
}

class EditWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EditWindow(QWidget *parent = 0);
    ~EditWindow();

private:
    Ui::EditWindow *ui;
    DetailsWindow* _detailsWindow;
    ManageProductWindow* _manageProductWindow;
};

#endif // EDITWINDOW_H
