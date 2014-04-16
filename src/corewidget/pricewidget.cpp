#include "pricewidget.h"
#include "ui_pricewidget.h"

#include <QList>
#include <QMap>
#include <QVariant>
#include <QStringList>
#include <QTime>

#include "globalsbase.h"

PriceWidget::PriceWidget(QWidget *parent) :
    MainWidget(parent),
    ui(new Ui::PriceWidget)
  , mId(-1)
{
    ui->setupUi(this);
    ui->mpPrice->setValidator(new QIntValidator(0, 2000000000));
    /*connect(ui->mpPrice, SIGNAL(textChanged(QString)), this, SLOT(save()));
    connect(ui->mpIf, SIGNAL(textChanged(QString)), this, SLOT(save()));*/

}

PriceWidget::~PriceWidget()
{
    delete ui;
}

void PriceWidget::load(int aIdObjects)
{
    MainWidget::load();
    mIdObjects = aIdObjects;
    ResponseType vResponse = execQuery(
                QString("SELECT id, price, condition FROM price WHERE object_fk = %1")
                .arg(mIdObjects));

    if (vResponse.count())
    {
        ResponseRecordType vQuery = vResponse[0];

        mId = vQuery["id"].toInt();
        ui->mpPrice->setText(vQuery["price"].toString());
        ui->mpIf->setText(vQuery["condition"].toString());
        MainWidget::save();
    }
    else
    {
        mId = -1;
        ui->mpPrice->setText("");
        ui->mpIf->setText("");
    }

}

void PriceWidget::save()
{
    if (mId < 0)
    {
        execQuery(QString("INSERT INTO price (object_fk) VALUES (%1)")
                  .arg(mIdObjects));
        mId = execQuery(
                    QString("SELECT id FROM price WHERE object_fk = %1")
                    .arg(mIdObjects))[0]["id"].toInt();
    }
    execQuery(QString("UPDATE price SET price = %1, condition = '%2' WHERE id = %3")
              .arg(ui->mpPrice->text().isEmpty() ? "0" : ui->mpPrice->text())
              .arg(ui->mpIf->text().isEmpty() ? "" : ui->mpIf->text())
              .arg(mId));
    MainWidget::save();
}

void PriceWidget::setEnable(bool aEnable)
{
    ui->mpIf->setEnabled(aEnable);
    ui->mpPrice->setEnabled(aEnable);
}
