#include "findinformationarea.h"
#include "ui_findinformationarea.h"

FindInformationArea::FindInformationArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FindInformationArea)
{
    ui->setupUi(this);
}

FindInformationArea::~FindInformationArea()
{
    delete ui;
}

int FindInformationArea::minTotal()
{
    return ui->mpMinTotal->text().toInt();
}

int FindInformationArea::maxTotal()
{
    return ui->mpMaxTotal->text().toInt();
}

int FindInformationArea::minFloor()
{
    return ui->mpMinFloor->text().toInt();
}

int FindInformationArea::maxFloor()
{
    return ui->mpMaxFloor->text().toInt();
}

int FindInformationArea::minKitchen()
{
    return ui->mpMinKitchen->text().toInt();
}

int FindInformationArea::maxKitchen()
{
    return ui->mpMaxKitchen->text().toInt();
}

QString FindInformationArea::sqlWhere()
{
    QStringList vWheres;
    vWheres
            << (minTotal() > 0 ? QString("area.total >= %1").arg(minTotal()) : QString(""))
            << (maxTotal() > 0 ? QString("area.total <= %1").arg(maxTotal()) : QString(""))
            << (minKitchen() > 0 ? QString("area.kitchen >= %1").arg(minKitchen()) : QString(""))
            << (maxKitchen() > 0 ? QString("area.kitchen <= %1").arg(maxKitchen()) : QString(""))
            << (minFloor() > 0 ? QString("area.floor >= %1").arg(minFloor()) : QString(""))
            << (maxFloor() > 0 ? QString("area.floor <= %1").arg(maxFloor()) : QString(""));

    vWheres.removeAll(QString(""));
    return QString("(%1)").arg(vWheres.join(QString(" AND ")));
}

QString FindInformationArea::joinWhere()
{
    return QString("INNER JOIN area ON objects.id = area.object_fk");
}
