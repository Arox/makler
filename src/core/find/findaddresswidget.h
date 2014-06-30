#ifndef FINDADDRESSWIDGET_H
#define FINDADDRESSWIDGET_H

#include "address/addresscity.h"

class FindAddressWidget : public AddressCity
{
    Q_OBJECT
public:
    explicit FindAddressWidget(QWidget *parent = 0);
    QString sqlWhere();
    QString joinWhere();
};

#endif // FINDADDRESSWIDGET_H
