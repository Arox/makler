#ifndef FINDADDRESSHOMEWIDGET_H
#define FINDADDRESSHOMEWIDGET_H

#include "addresshomewidget.h"

class FindAddressHomeWidget : public AddressHomeWidget
{
    Q_OBJECT
public:
    explicit FindAddressHomeWidget(QWidget *parent = 0);
    QString sqlWhere();
    QString joinWhere();
};

#endif // FINDADDRESSHOMEWIDGET_H
