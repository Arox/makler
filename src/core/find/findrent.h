#ifndef FINDRENT_H
#define FINDRENT_H

#include <QWidget>
#include "widgetforcontrol.h"
#include "findinformationarea.h"
#include "findpricewidget.h"
#include "findtypeapartment.h"
#include "apartmentinformation.h"

#include "findwidget.h"
#include "address/address.h"

class FindRent : public FindWidget
{
    Q_OBJECT
    
public:
    explicit FindRent(QWidget *parent = 0);
    ~FindRent();
protected:
    virtual void getData();
private:
    Address mAddress;
    FindInformationArea mArea;
    FindPriceWidget mPrice;
    FindTypeApartment mType;
    OptionForApartment mInformation;
};

#endif // FINDRENT_H
