#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include "mainwidget.h"

#include <QWidget>

namespace Ui {
class HelpWidget;
}

class HelpWidget : public MainWidget
{
    Q_OBJECT

public:
    explicit HelpWidget(QWidget *parent = 0);
    ~HelpWidget();

private slots:
    void on_mpExit_clicked();

private:
    Ui::HelpWidget *ui;
};

#endif // HELPWIDGET_H
