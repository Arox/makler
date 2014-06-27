#ifndef ADDRESSWIDGET_H
#define ADDRESSWIDGET_H

#include <QWidget>
#include <QList>
#include <QComboBox>
#include <QSignalMapper>
#include "../mainwidget.h"

#include "verticalcontanerwidget.h"

namespace Ui {
class AddressWidget;
}

class AddressWidget : public MainWidget
{
    Q_OBJECT
    
public:
    explicit AddressWidget(int aState, QWidget *parent = 0);
    ~AddressWidget();
    void load(int aIdObjects, int aNumber);
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

#endif // ADDRESSWIDGET_H
