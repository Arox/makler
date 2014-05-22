#include "dialoguniversal.h"
#include "ui_dialoguniversal.h"
#include "mainwidget.h"
DialogUniversal::DialogUniversal(QWidget* apChildren, QString aName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUniversal)
{
    ui->setupUi(this);
    setWindowTitle(aName);
    MainWidget* vpWidget = (MainWidget*)apChildren;
    ui->mainLayout->addWidget(apChildren);
    connect(vpWidget, SIGNAL(back()), this, SLOT(accept()));
    resize(apChildren->width(), apChildren->height());
}

DialogUniversal::~DialogUniversal()
{
    delete ui;
}
