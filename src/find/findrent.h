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

namespace Ui {
class FindRent;
}

class FindRent : public WidgetForControl
{
    Q_OBJECT
    
public:
    explicit FindRent(QWidget *parent = 0);
    ~FindRent();
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
    Ui::FindRent *ui;
    FindAddressWidget mAddress;
    FindAddressHomeWidget mHomeAddress;
    FindInformationArea mArea;
    FindPriceWidget mPrice;
    FindTypeApartment mType;
    ApartmentInformation mInformation;

    QString mSql;
    QString mJoin;
};

#endif // FINDRENT_H
