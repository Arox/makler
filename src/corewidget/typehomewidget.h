#ifndef TYPEHOMEWIDGET_H
#define TYPEHOMEWIDGET_H

#include <QWidget>
#include "informationarea.h"
#include "mainwidget.h"
namespace Ui {
class TypeHomeWidget;
}

class TypeHomeWidget : public MainWidget
{
    Q_OBJECT
    
public:
    explicit TypeHomeWidget(QWidget *parent = 0);
    ~TypeHomeWidget();
    void load(int aIdObjects);
    void setEnable(bool aEnable);
public slots:
    void save();
private:
    Ui::TypeHomeWidget *ui;
    int mId;
    int mIdObjects;
    //InformationArea mInfoArea;
};

#endif // TYPEHOMEWIDGET_H
