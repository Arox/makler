#ifndef FINDHOME_H
#define FINDHOME_H

#include <QWidget>
#include "widgetforcontrol.h"
#include "findaddresshomewidget.h"
#include "findinformationarea.h"
#include "findpricewidget.h"
#include "findtypehome.h"
#include "apartmentinformation.h"
#include "findwidget.h"

class FindHome : public FindWidget
{
    Q_OBJECT
    
public:
    explicit FindHome(QWidget *parent = 0);
    ~FindHome();
protected:
    virtual void getData();
private:
    FindAddressHomeWidget mHomeAddress;
    FindInformationArea mArea;
    FindPriceWidget mPrice;
    FindTypeHome mType;
    OptionForApartment mInformation;
};

#endif // FINDHOME_H
