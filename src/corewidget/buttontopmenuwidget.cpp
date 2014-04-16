#include "buttontopmenuwidget.h"
#include "ui_buttontopmenuwidget.h"

ButtonTopMenuWidget::ButtonTopMenuWidget(QString aText, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ButtonTopMenuWidget)
{
    ui->setupUi(this);
    ui->label->setText(aText);
    ui->label->setAlignment(Qt::AlignHCenter);

    connect(ui->pushButton, SIGNAL(clicked()), this, SIGNAL(activated()));
}

ButtonTopMenuWidget::~ButtonTopMenuWidget()
{
    delete ui;
}

void ButtonTopMenuWidget::setActive(bool aFlag)
{
    ui->pushButton->setChecked(aFlag);
}
