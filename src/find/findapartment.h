#ifndef FINDAPARTMENT_H
#define FINDAPARTMENT_H

#include <QWidget>
#include "widgetforcontrol.h"
#include "findaddresswidget.h"
#include "findinformationarea.h"
#include "findpricewidget.h"
#include "findtypeapartment.h"
#include "apartmentinformation.h"
#include "findwidget.h"

class FindApartment : public FindWidget
{
    Q_OBJECT
    
public:
    explicit FindApartment(QWidget *parent = 0);
    ~FindApartment();
protected:
    void getData();
private:
    FindAddressWidget mAddress;
    FindInformationArea mArea;
    FindPriceWidget mPrice;
    FindTypeApartment mType;
    ApartmentInformation mInformation;
};

#endif // FINDAPARTMENT_H
