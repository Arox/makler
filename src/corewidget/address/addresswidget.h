#ifndef ADDRESSWIDGET_H
#define ADDRESSWIDGET_H

#include <QWidget>
#include <QList>
#include <QComboBox>
#include <QSignalMapper>
#include "mainwidget.h"

namespace Ui {
class AddressWidget;
}

class AddressWidget : public MainWidget
{
    Q_OBJECT
    
public:
    enum StateWidget
    {
        NORMAL = 1,
        FIND = 128,
        FINDROOM = 192,
        MULTISELECT = 8
    };
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
    void loadCity();
    void loadLocality();
    void loadStreet();
    void save();
    void saveCity();
    void saveLocality();
    void saveStreet();
private slots:
    void on_mpCity_currentIndexChanged(int index);
protected:
    QList<int> idsComboBox(QComboBox* apBox);
    QList<int> idComboBox(QComboBox* apBox);
    void clickItemComboBox(QComboBox* apBox, int aIndex);

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
    QList<QComboBox*> mBoxes;
};

#endif // ADDRESSWIDGET_H
