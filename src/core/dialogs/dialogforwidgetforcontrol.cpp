#include "dialogforwidgetforcontrol.h"
#include "ui_dialogforwidgetforcontrol.h"

#include "widgetforcontrol.h"

DialogForWidgetForControl::DialogForWidgetForControl(QWidget* apChildren, QString aName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogForWidgetForControl)
{
    ui->setupUi(this);
    setWindowTitle(aName);
    WidgetForControl* vpWidget = (WidgetForControl*)apChildren;
    ui->mainLayout->addWidget(apChildren);
    connect(vpWidget, SIGNAL(back(WidgetForControl*)), this, SLOT(accept()));
    resize(apChildren->width(), apChildren->height());
}

DialogForWidgetForControl::~DialogForWidgetForControl()
{
    delete ui;
}
