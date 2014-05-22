#ifndef FINDRENT_H
#define FINDRENT_H

#include <QWidget>
#include "widgetforcontrol.h"
#include "findaddresswidget.h"
#include "findaddresshomewidget.h"
#include "findinformationarea.h"
#include "findpricewidget.h"
#include "findtypeapartment.h"
#include "apartmentinformation.h"

#include "findwidget.h"

class FindRent : public FindWidget
{
    Q_OBJECT
    
public:
    explicit FindRent(QWidget *parent = 0);
    ~FindRent();
protected:
    virtual void getData();
private:
    FindAddressWidget mAddress;
    FindAddressHomeWidget mHomeAddress;
    FindInformationArea mArea;
    FindPriceWidget mPrice;
    FindTypeApartment mType;
    ApartmentInformation mInformation;
};

#endif // FINDRENT_H
