#ifndef DETAILSWINDOW_H
#define DETAILSWINDOW_H

#include <QWidget>

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
};

#endif // DETAILSWINDOW_H
