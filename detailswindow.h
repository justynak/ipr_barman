#ifndef DETAILSWINDOW_H
#define DETAILSWINDOW_H

#include <QWidget>
#include "order.h"

namespace Ui {
class DetailsWindow;
}

class DetailsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DetailsWindow(QWidget *parent = 0);
    ~DetailsWindow();

private:
    Ui::DetailsWindow *ui;

signals:
    void closeBill();

};

#endif // DETAILSWINDOW_H
