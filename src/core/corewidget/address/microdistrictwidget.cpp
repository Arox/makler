#include "microdistrictwidget.h"

MicrodistrictWidget::MicrodistrictWidget(QWidget *parent) :
    LocationSelect(parent)
  , mpModel(0)
{
}

void MicrodistrictWidget::load(int aCityFk)
{
    if (mpModel) delete mpModel;
    mpModel = new MicrodistrictModel(aCityFk, this);
    LocationSelect::load();
}

LocationModel* MicrodistrictWidget::modelSelect()
{
    return mpModel;
}

LocationModel* MicrodistrictWidget::modelCompleter()
{
    return mpModel;
}
