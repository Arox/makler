#ifndef TYPEAPARTMENT_H
#define TYPEAPARTMENT_H

#include <QWidget>

#include "mainwidget.h"

namespace Ui {
class TypeApartment;
}

class TypeApartment : public MainWidget
{
    Q_OBJECT
    
public:
    explicit TypeApartment(QWidget *parent = 0);
    ~TypeApartment();
    void load(int aIdObjects);
    void setEnable(bool aEnable);
public slots:
    void save();
private:
    Ui::TypeApartment *ui;
    int mId;
    int mIdObjects;
};

#endif // TYPEAPARTMENT_H
