#ifndef MANAGEPRODUCTWINDOW_H
#define MANAGEPRODUCTWINDOW_H

#include <QWidget>

namespace Ui {
class ManageProductWindow;
}

class ManageProductWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ManageProductWindow(QWidget *parent = 0);
    ~ManageProductWindow();

private:
    Ui::ManageProductWindow *ui;
};

#endif // MANAGEPRODUCTWINDOW_H
