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

private:
    Ui::EditWindow *ui;
    DetailsWindow* _detailsWindow;
    Bartender* _bartender;
};

#endif // EDITWINDOW_H
