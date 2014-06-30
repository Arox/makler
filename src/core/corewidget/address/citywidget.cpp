#include "citywidget.h"

#include <QCompleter>

CityWidget::CityWidget(CityModel::TypeCity aType, QWidget *parent) :
    LocationSelect(parent)
  , mModel(aType)
{
    load();
}

LocationModel* CityWidget::modelSelect()
{
    return &mModel;
}

LocationModel* CityWidget::modelCompleter()
{
    return &mModel;
}

CityModel::TypeCity CityWidget::type()
{
    return mModel.type();
}
