#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include "clientheaderwidget.h"
#include "address/addresshome.h"
#include "informationarea.h"
#include "pricewidget.h"
#include "typehomewidget.h"
#include "commentwidget.h"
#include "buttonswidget.h"
#include "apartmentinformation.h"
#include "generalwidget.h"


namespace Ui {
class HomeWidget;
}

class HomeWidget : public GeneralWidget
{
    Q_OBJECT
    
public:
    explicit HomeWidget(int aAgent, QWidget *parent = 0);
    void load(int aId = -1);
    ~HomeWidget();
    virtual QString name();
    virtual QSize minSize();
    virtual QSize maxSize();
    virtual QSize size();
    virtual QString idName();
private slots:
    void backWidget();
    void noSave();
private:
    Ui::HomeWidget *ui;
    ClientHeaderWidget mClient;
    AddressHome mAddress;
    InformationArea mArea;
    PriceWidget mPrice;
    TypeHomeWidget mType;
    OptionForApartment mInformation;
    CommentWidget mComment;
    ButtonsWidget mButtons;

    int mId;
    int mAgent;
    bool mIsLoad;
};

#endif // HOMEWIDGET_H
