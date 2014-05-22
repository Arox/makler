#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent):
    QWidget(parent)
  ,mIsNew(true)
{
}

MainWidget::~MainWidget()
{

}

bool MainWidget::isSave()
{
    return mIsNew;
}

void MainWidget::load()
{
    mIsNew = true;
}

void MainWidget::save()
{
    mIsNew = false;
}
