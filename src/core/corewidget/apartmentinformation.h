#ifndef APARTMENTINFORMATION_H
#define APARTMENTINFORMATION_H

#include <QWidget>
#include <QComboBox>
#include <QList>
#include "mainwidget.h"

namespace Ui {
class ApartmentInformation;
}

class SelectForOptionForApartment: public QComboBox
{
    Q_OBJECT
public:
    explicit SelectForOptionForApartment(int aId, QWidget *parent = 0);
    ~SelectForOptionForApartment();
    void load(int aIdObject);
    int value();
    QList<int> values();
    void setMultiSelect(bool aFlag);
    bool isMultiSelect();
public slots:
    void save();
    void setItem(int aIndex);
protected:
    void setConnect();
    void setDisconnect();
private:
    int mId;
    int mIdValue;
    int mIdObject;
    bool mMultiSelect;
};
/*
class BrainLineEdit: public QLineEdit
{
    Q_OBJECT
public:
    explicit BrainLineEdit(int aId, QWidget *parent = 0);
    ~BrainLineEdit();
    void load(int aIdObject);
    int value();
public slots:
    void save();
protected:
    void setConnect();
    void setDisconnect();
private:
    int mId;
    int mIdValue;
    int mIdObject;
};
*/
class OptionForApartment : public MainWidget
{
    Q_OBJECT

public:
    explicit OptionForApartment(QWidget *parent = 0);
    ~OptionForApartment();
    void setNameDictionary(QString aName, int aCountColumn, bool aMultiSelect = false);
    void load(int aIdObject);
    virtual bool isSave();
    QString sqlWhere();
    QString joinWhere();
public slots:
    void save();
private:
    Ui::ApartmentInformation *ui;
    int mIdObject;
    QList<SelectForOptionForApartment*> mBoxes;
};

#endif // APARTMENTINFORMATION_H
