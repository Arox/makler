#ifndef ADDRESS_H
#define ADDRESS_H

#include "addresscity.h"
#include "addresshome.h"

#include "baseaddresswidget.h"

#include <QWidget>

namespace Ui {
class Address;
}

class Address : public QWidget
{
    Q_OBJECT

public:
    Address(int aState, QWidget *parent = 0);
    ~Address();
    BaseAddressWidget& data();
    bool canSave();
    void setEnabled(bool);
    bool isEnabled() const;
public slots:
    void save();
    void load(int aId);
private slots:
    void on_mpCity_clicked();
    void on_mpGarden_clicked();
private:
    Ui::Address *ui;
    AddressCity mAddressInCity;
    AddressHome mAddressInGarden;
};

#endif // ADDRESS_H
