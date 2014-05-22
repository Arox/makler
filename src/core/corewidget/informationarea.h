#ifndef INFORMATIONAREA_H
#define INFORMATIONAREA_H

#include <QWidget>
#include "mainwidget.h"
namespace Ui {
class InformationArea;
}

class InformationArea : public MainWidget
{
    Q_OBJECT
    
public:
    explicit InformationArea(QWidget *parent = 0);
    ~InformationArea();
    void load(int aIdObjects);
public slots:
    void save();
private:
    Ui::InformationArea *ui;
    int mId;
    int mIdObjects;
};

#endif // INFORMATIONAREA_H
