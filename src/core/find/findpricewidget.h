#ifndef FINDPRICEWIDGET_H
#define FINDPRICEWIDGET_H

#include <QWidget>

namespace Ui {
class FindPriceWidget;
}

class FindPriceWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit FindPriceWidget(QWidget *parent = 0);
    ~FindPriceWidget();
    int min();
    int max();
    QString sqlWhere();
    QString joinWhere();
private:
    Ui::FindPriceWidget *ui;
};

#endif // FINDPRICEWIDGET_H
