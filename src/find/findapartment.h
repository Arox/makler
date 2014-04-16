#ifndef FINDAPARTMENT_H
#define FINDAPARTMENT_H

#include <QWidget>
#include "widgetforcontrol.h"
#include "findaddresswidget.h"
#include "findinformationarea.h"
#include "findpricewidget.h"
#include "findtypeapartment.h"
#include "apartmentinformation.h"

namespace Ui {
class FindApartment;
}

class FindApartment : public WidgetForControl
{
    Q_OBJECT
    
public:
    explicit FindApartment(QWidget *parent = 0);
    ~FindApartment();
    QString sql();
    QString join();
    virtual QString name();
    virtual QSize minSize();
    virtual QSize maxSize();
    virtual QSize size();
    virtual QString idName();
private slots:
    void on_mpFind_clicked();

    void on_mpBack_clicked();

private:
    Ui::FindApartment *ui;
    FindAddressWidget mAddress;
    FindInformationArea mArea;
    FindPriceWidget mPrice;
    FindTypeApartment mType;
    ApartmentInformation mInformation;

    QString mSql;
    QString mJoin;
};

#endif // FINDAPARTMENT_H
