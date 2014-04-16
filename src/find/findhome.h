#ifndef FINDHOME_H
#define FINDHOME_H

#include <QWidget>
#include "widgetforcontrol.h"
#include "findaddresswidget.h"
#include "findaddresshomewidget.h"
#include "findinformationarea.h"
#include "findpricewidget.h"
#include "findtypehome.h"
#include "apartmentinformation.h"

namespace Ui {
class FindHome;
}

class FindHome : public WidgetForControl
{
    Q_OBJECT
    
public:
    explicit FindHome(QWidget *parent = 0);
    ~FindHome();
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
    Ui::FindHome *ui;
    FindAddressWidget mAddress;
    FindAddressHomeWidget mHomeAddress;
    FindInformationArea mArea;
    FindPriceWidget mPrice;
    FindTypeHome mType;
    ApartmentInformation mInformation;

    QString mSql;
    QString mJoin;
};

#endif // FINDHOME_H
