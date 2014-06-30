#ifndef BASEADDRESSWIDGET_H
#define BASEADDRESSWIDGET_H

#include <QWidget>
#include <QList>
#include <QComboBox>
#include <QSignalMapper>
#include "../mainwidget.h"

#include "verticalcontanerwidget.h"
#include "citymodel.h"

namespace Ui {
class AddressWidget;
}

class BaseAddressWidget : public MainWidget
{
    Q_OBJECT
    
public:
    explicit BaseAddressWidget(CityModel::TypeCity aType, int aState, QWidget *parent = 0);
    ~BaseAddressWidget();
    void load(int aIdObjects);
     bool canSave();
public:


    QList<int> city();
    QList<int> locality();
    QList<int> street();
    int number1();
    int number2();
    int room();
    QString landmark();

    bool isCity();
    bool isLocality();
    bool isStreet();
    bool isNumber1();
    bool isNumber2();
    bool isRoom();
    bool isLandmark();


public slots:
    void save();
private slots:
    void on_mpCity_currentIndexChanged(int index);
protected slots:
    void selectComboBox(int aIndex);
private:
    void setEnabledWidgets(bool aEnable);
    void setState(int aState);
    bool isState(int aState);
    void setNormalVisible();
private:
    Ui::AddressWidget *ui;
    int mId;
    int mIdObjects;
    bool mEnabled;

    int mState;
    QSignalMapper mMapper;
    QList<LocationSelect*> mBoxes;

    VerticalContanerWidget mCityBox;
    VerticalContanerWidget mMicrodistrictBox;
    VerticalContanerWidget mStreetBox;
};

#endif // BASEADDRESSWIDGET_H
