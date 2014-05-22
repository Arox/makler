#ifndef PRICEWIDGET_H
#define PRICEWIDGET_H

#include <QWidget>

#include "mainwidget.h"

namespace Ui {
class PriceWidget;
}

class PriceWidget : public MainWidget
{
    Q_OBJECT
    
public:
    explicit PriceWidget(QWidget *parent = 0);
    ~PriceWidget();
    void load(int aIdObjects);
public slots:
    void save();
private:
    Ui::PriceWidget *ui;
    int mId;
    int mIdObjects;
};

#endif // PRICEWIDGET_H
