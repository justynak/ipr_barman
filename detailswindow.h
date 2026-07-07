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
    DetailsWindow(DraftOrder draft, QString currency, QWidget *parent = nullptr);
    ~DetailsWindow() override;

private:
    Ui::DetailsWindow *ui;
    DraftOrder _draft;
    QString _currency;


};

#endif // DETAILSWINDOW_H
