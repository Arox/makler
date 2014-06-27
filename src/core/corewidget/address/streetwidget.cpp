#include "streetwidget.h"

StreetWidget::StreetWidget(QWidget *parent) :
    LocationSelect(parent)
  , mpModel(0)
{
}

void StreetWidget::load(int aCityFk)
{
    if (mpModel) delete mpModel;
    mpModel = new StreetModel(aCityFk, this);
    LocationSelect::load();
}

LocationModel* StreetWidget::modelSelect()
{
    return mpModel;
}

LocationModel* StreetWidget::modelCompleter()
{
    return mpModel;
}
