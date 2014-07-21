#ifndef FINDADDRESSHOMEWIDGET_H
#define FINDADDRESSHOMEWIDGET_H

#include "address/baseaddresswidget.h"

class FindAddressHomeWidget : public BaseAddressWidget
{
    Q_OBJECT
public:
    explicit FindAddressHomeWidget(QWidget *parent = 0);
    QString sqlWhere();
    QString joinWhere();
};

#endif // FINDADDRESSHOMEWIDGET_H
