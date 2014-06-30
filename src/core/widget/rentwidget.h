#ifndef RENTWIDGET_H
#define RENTWIDGET_H

#include <QWidget>
#include "clientheaderwidget.h"
#include "address/address.h"
#include "address/addresshomewidget.h"
#include "informationarea.h"
#include "pricewidget.h"
#include "typeapartment.h"
#include "commentwidget.h"
#include "buttonswidget.h"
#include "apartmentinformation.h"
#include "generalwidget.h"

namespace Ui {
class RentWidget;
}

class RentWidget : public GeneralWidget
{
    Q_OBJECT
    
public:
    explicit RentWidget(int aAgent, QWidget *parent = 0);
    void load(int aId = -1);
    ~RentWidget();
    virtual QString name();
    virtual QSize minSize();
    virtual QSize maxSize();
    virtual QSize size();
    virtual QString idName();
private slots:
    void backWidget();
    void noSave();
private:
    Ui::RentWidget *ui;
    ClientHeaderWidget mClient;
    Address mAddress;
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

#endif // RENTWIDGET_H
