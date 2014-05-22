#ifndef FINDADDRESSWIDGET_H
#define FINDADDRESSWIDGET_H

#include "address/addresswidget.h"

class FindAddressWidget : public AddressWidget
{
    Q_OBJECT
public:
    explicit FindAddressWidget(QWidget *parent = 0);
    QString sqlWhere();
    QString joinWhere();
};

#endif // FINDADDRESSWIDGET_H
