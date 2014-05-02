#include "findpricewidget.h"
#include "ui_findpricewidget.h"

FindPriceWidget::FindPriceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FindPriceWidget)
{
    ui->setupUi(this);
}

FindPriceWidget::~FindPriceWidget()
{
    delete ui;
}

int FindPriceWidget::min()
{
    return ui->mpMin->text().toInt();
}

int FindPriceWidget::max()
{
    return ui->mpMax->text().toInt();
}

QString FindPriceWidget::sqlWhere()
{
    QStringList vWheres;
    vWheres
            << (min() > 0 ? QString("price.price >= %1").arg(min()) : QString(""))
            << (max() > 0 ? QString("price.price <= %1").arg(max()) : QString(""));

    vWheres.removeAll(QString(""));
    return QString("(%1)").arg(vWheres.join(QString(" AND ")));
}

QString FindPriceWidget::joinWhere()
{
    return QString("INNER JOIN price ON objects.id = price.object_fk");
}
