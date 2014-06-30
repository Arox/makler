#ifndef CITYWIDGET_H
#define CITYWIDGET_H

#include "locationselect.h"
#include "citymodel.h"

class CityWidget : public LocationSelect
{
    Q_OBJECT
    //CityModel mModelCompleter;
    CityModel mModel;
public:
    explicit CityWidget(CityModel::TypeCity aType, QWidget *parent = 0);
    CityModel::TypeCity type();
protected:
    virtual LocationModel* modelSelect();
    virtual LocationModel* modelCompleter();
signals:

public slots:

};

#endif // CITYWIDGET_H
