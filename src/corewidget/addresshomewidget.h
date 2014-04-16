#ifndef ADDRESSHOMEWIDGET_H
#define ADDRESSHOMEWIDGET_H

#include <QWidget>
#include <QList>
#include <QComboBox>
#include <QSignalMapper>
#include "mainwidget.h"

namespace Ui {
class AddressHomeWidget;
}

class AddressHomeWidget : public MainWidget
{
    Q_OBJECT
    
public:
    explicit AddressHomeWidget(int aState, QWidget *parent = 0);
    ~AddressHomeWidget();
    void load(int aIdObjects, int aNumber);
    void setEnable(bool aEnable);
    bool canSave();
public:
    enum StateWidget
    {
        NORMAL = 0,
        FIND = 1,
        FINDHOME = 4,
        MULTISELECT = 8
    };

    QList<int> garden();
    QList<int> street();
    int number1();
    int number2();

    bool isGarden();
    bool isStreet();
    bool isNumber1();
    bool isNumber2();


public slots:
    void saveGarden();
    void saveStreet();
    void loadGarden();
    void loadStreet();
    void save();
private slots:
    //void on_mpStreet_textChanged(const QString &arg1);
    void on_mpGarden_currentIndexChanged(int index);

    void on_mpStreet_currentIndexChanged(int index);
protected:
    QList<int> idsComboBox(QComboBox* apBox);
    void clickItemComboBox(QComboBox* apBox, int aIndex);
protected slots:
    void selectComboBox(int aIndex);
private:
    void setEnabledWidgets(bool aEnable);
    void setState(int aState);
private:
    Ui::AddressHomeWidget *ui;
    bool mEnabled;
    int mId;
    int mIdObjects;
    int mState;
    QSignalMapper mMapper;
    QList<QComboBox*> mBoxes;
};

#endif // ADDRESSHOMEWIDGET_H
