#include "informationarea.h"
#include "ui_informationarea.h"

#include <QList>
#include <QMap>
#include <QVariant>
#include <QStringList>
#include <QTime>

#include "globalsbase.h"

InformationArea::InformationArea(QWidget *parent) :
    MainWidget(parent),
    ui(new Ui::InformationArea)
  , mId(-1)
{
    ui->setupUi(this);
    ui->label_5->setProperty("title", "true");
    /*connect(ui->mpTotal, SIGNAL(textChanged(QString)), this, SLOT(save()));
    connect(ui->mpFloor, SIGNAL(textChanged(QString)), this, SLOT(save()));
    connect(ui->mpKitchen, SIGNAL(textChanged(QString)), this, SLOT(save()));*/
}

InformationArea::~InformationArea()
{
    delete ui;
}

void InformationArea::load(int aIdObjects)
{
    MainWidget::load();
    mIdObjects = aIdObjects;
    ResponseType vResponse = execQuery(
                QString("SELECT id, total, floor, kitchen FROM area WHERE object_fk = %1")
                .arg(mIdObjects));
    if (vResponse.count())
    {
        ResponseRecordType vQuery = vResponse[0];

        ui->mpFloor->blockSignals(true);
        ui->mpKitchen->blockSignals(true);
        ui->mpTotal->blockSignals(true);
        mId = vQuery["id"].toInt();
        ui->mpTotal->setText(vQuery["total"].toString());
        ui->mpKitchen->setText(vQuery["kitchen"].toString());
        ui->mpFloor->setText(vQuery["floor"].toString());
        ui->mpFloor->blockSignals(false);
        ui->mpKitchen->blockSignals(false);
        ui->mpTotal->blockSignals(false);
        MainWidget::save();
    }
    else
    {
        mId = -1;
        ui->mpTotal->setText("");
        ui->mpKitchen->setText("");
        ui->mpFloor->setText("");
    }

}

void InformationArea::save()
{
    if (mId < 0)
    {
        execQuery(QString("INSERT INTO area (object_fk) VALUES (%1)")
                  .arg(mIdObjects));
        mId = execQuery(
                    QString("SELECT id FROM area WHERE object_fk = %1")
                    .arg(mIdObjects))[0]["id"].toInt();
    }
    execQuery(QString("UPDATE area SET total = %1, floor = %2, kitchen = %3 WHERE id = %4")
              .arg(ui->mpTotal->text().isEmpty() ? "0" : ui->mpTotal->text())
              .arg(ui->mpFloor->text().isEmpty() ? "0" : ui->mpFloor->text())
              .arg(ui->mpKitchen->text().isEmpty() ? "0" : ui->mpKitchen->text())
              .arg(mId));
    MainWidget::save();
}
