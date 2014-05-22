#ifndef ABOUTPRODUCERWIDGET_H
#define ABOUTPRODUCERWIDGET_H

#include "mainwidget.h"

#include <QWidget>

namespace Ui {
class AboutProducerWidget;
}

class AboutProducerWidget : public MainWidget
{
    Q_OBJECT

public:
    explicit AboutProducerWidget(QWidget *parent = 0);
    ~AboutProducerWidget();

private slots:
    void on_mpExit_clicked();

private:
    Ui::AboutProducerWidget *ui;
};

#endif // ABOUTPRODUCERWIDGET_H
