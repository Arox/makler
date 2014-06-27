#ifndef LOCATIONSELECT_H
#define LOCATIONSELECT_H

#include "locationmodel.h"
#include "generalsettingslocation.h"

#include <QComboBox>

class LocationSelect : public QComboBox
{
    Q_OBJECT
    int mStatus;
public:


    explicit LocationSelect(QWidget *parent = 0);
    void setType(int aType);
    bool isType(location::TypeSelect aType);
    QList<int> values();
public slots:
    void load();
    void reload();
    void clickItemComboBox(int aIndex);
protected:
    virtual LocationModel* modelSelect() = 0;
    virtual LocationModel* modelCompleter() = 0;
    QList<int> idsComboBox();
    QList<int> idComboBox();
private slots:
    void on_changeStatus(int aType);
signals:
    void changeType(int);
};

#endif // LOCATIONSELECT_H
