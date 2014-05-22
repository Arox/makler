#ifndef ADDRESS_H
#define ADDRESS_H

#include "addresswidget.h"
#include "addresshomewidget.h"

#include <QWidget>

namespace Ui {
class Address;
}

class Address : public QWidget
{
    Q_OBJECT

public:
    enum TypeAddress
    {
        CITY = 1,
        GARDEN = 2
    };

    enum StateWidget
    {
        NORMAL = 1,
        FIND = 128,
        FINDROOM = 192,
        FINDHOME = 160,
        MULTISELECT = 8
    };

    explicit Address(int aType, int aState, QWidget *parent = 0);
    ~Address();
    void load(int aIdObjects, int aNumber);
    bool canSave();

public slots:
    void save();
private:
    void setType(int aType, int aState);
private:
    Ui::Address *ui;
    AddressWidget* mpAddressInCity;
    AddressHomeWidget*mpAddressInGarden;
};

#endif // ADDRESS_H
