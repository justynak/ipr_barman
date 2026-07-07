#ifndef DETAILSWINDOW_H
#define DETAILSWINDOW_H

#include <QWidget>
#include <QDialog>

#include "draftorder.h"


namespace Ui {
class DetailsWindow;
}

class DetailsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DetailsWindow(DraftOrder draft, QWidget *parent = 0);
    ~DetailsWindow();

private slots:
    void on_button_ok_clicked();
    void on_button_discard_clicked();

private:
    Ui::DetailsWindow *ui;
    DraftOrder _draft;


};

#endif // DETAILSWINDOW_H
