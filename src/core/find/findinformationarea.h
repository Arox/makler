#ifndef FINDINFORMATIONAREA_H
#define FINDINFORMATIONAREA_H

#include <QWidget>

namespace Ui {
class FindInformationArea;
}

class FindInformationArea : public QWidget
{
    Q_OBJECT
    
public:
    explicit FindInformationArea(QWidget *parent = 0);
    ~FindInformationArea();
    int minTotal();
    int maxTotal();
    int minFloor();
    int maxFloor();
    int minKitchen();
    int maxKitchen();
    QString sqlWhere();
    QString joinWhere();
private:
    Ui::FindInformationArea *ui;
};

#endif // FINDINFORMATIONAREA_H
