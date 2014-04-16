#include "messagewidget.h"
#include "ui_messagewidget.h"

MessageWidget::MessageWidget(int aUser_fk, QWidget *parent) :
    WidgetForControl(parent),
    ui(new Ui::MessageWidget),
    mUser_fk(aUser_fk),
    mViewPost(aUser_fk)
{
    ui->setupUi(this);
    ui->mainLayout->addWidget(&mViewPost);


}

MessageWidget::~MessageWidget()
{
    delete ui;
}

QString MessageWidget::name()
{
    return TRANSLATE("Сообщения");
}

QSize MessageWidget::minSize()
{
    return QSize(0, 0);
}

QSize MessageWidget::size()
{
    return QSize(787, 800);
}

QSize MessageWidget::maxSize()
{
    return QSize(0, 0);
}

QString MessageWidget::idName()
{
    return "messages";
}
