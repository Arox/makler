#ifndef APARTMENTWIDGET_H
#define APARTMENTWIDGET_H

#include <QWidget>
#include "generalwidget.h"
#include "clientheaderwidget.h"
#include "address/addresswidget.h"
#include "informationarea.h"
#include "pricewidget.h"
#include "typeapartment.h"
#include "commentwidget.h"
#include "buttonswidget.h"
#include "apartmentinformation.h"

namespace Ui {
class ApartmentWidget;
}

class ApartmentWidget : public GeneralWidget
{
    Q_OBJECT
    
public:
    explicit ApartmentWidget(int aAgent, QWidget *parent = 0);
    ~ApartmentWidget();
    void load(int aId = -1);
    virtual QString name();
    virtual QSize minSize();
    virtual QSize maxSize();
    virtual QSize size();
    virtual QString idName();
private slots:
    void backWidget();
    void noSave();
private:
    Ui::ApartmentWidget *ui;
    ClientHeaderWidget mClient;
    AddressWidget mAddress;
    InformationArea mArea;
    PriceWidget mPrice;
    TypeApartment mType;
    OptionForApartment mInformation;
    CommentWidget mComment;
    ButtonsWidget mButtons;

    int mId;
    int mAgent;
    bool mIsLoad;
};

#endif // APARTMENTWIDGET_H
