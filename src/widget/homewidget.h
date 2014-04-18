#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include "clientheaderwidget.h"
#include "addresswidget.h"
#include "informationarea.h"
#include "pricewidget.h"
#include "typehomewidget.h"
#include "commentwidget.h"
#include "buttonswidget.h"
#include "apartmentinformation.h"
#include "generalwidget.h"
#include "addresshomewidget.h"

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
    AddressWidget mAddress;
    InformationArea mArea;
    PriceWidget mPrice;
    TypeHomeWidget mType;
    ApartmentInformation mInformation;
    CommentWidget mComment;
    ButtonsWidget mButtons;
    AddressHomeWidget mHomeAddress1;

    int mId;
    int mAgent;
    bool mIsLoad;
};

#endif // HOMEWIDGET_H
