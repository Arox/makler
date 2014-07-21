#include "findwidget.h"
#include "ui_findwidget.h"

#include "language.h"

FindWidget::FindWidget(QWidget *parent) :
    WidgetForControl(parent),
    ui(new Ui::FindWidget)
{
    ui->setupUi(this);
    ui->mpFind->setProperty("color", "true");
    connect(ui->mpFind, SIGNAL(clicked()), this, SLOT(on_mpFind_clicked()));
    connect(ui->mpBack, SIGNAL(clicked()), this, SLOT(on_mpBack_clicked()));
}

FindWidget::~FindWidget()
{
    delete ui;
}

QString FindWidget::sql()
{
    return mSql;
}

QString FindWidget::join()
{
    return mJoin;
}

QString FindWidget::name()
{
    return TRANSLATE("поиск");
}

QSize FindWidget::minSize()
{
    return QSize();
}

QSize FindWidget::maxSize()
{
    return QSize();
}

QSize FindWidget::size()
{
    return QSize();
}

QString FindWidget::idName()
{
    return "find";
}

void FindWidget::clearFind()
{
    mSql.clear();
}

void FindWidget::setSql(QString aSql)
{
    mSql = aSql;
}

void FindWidget::setJoin(QString aJoin)
{
    mJoin = aJoin;
}

void FindWidget::getBack()
{

}

void FindWidget::on_mpFind_clicked()
{
    getData();
    emit back(this);
}

void FindWidget::on_mpBack_clicked()
{
    getBack();
    emit back(this);
}

void FindWidget::addWidget(QWidget* apWidget)
{
    ui->mainLayout->insertWidget(0, apWidget);
}
