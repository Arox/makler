#include "passportwidget.h"
#include "ui_passportwidget.h"

#include "globals.h"
#include "globalsbase.h"

PassportWidget::PassportWidget(QWidget *parent) :
    MainWidget(parent),
    ui(new Ui::PassportWidget)
  , mId(-1)
{
    ui->setupUi(this);
    connect(ui->mpSave, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->mpBack, SIGNAL(clicked()), this, SIGNAL(back()));
}

PassportWidget::~PassportWidget()
{
    delete ui;
}

void PassportWidget::load(int aIdObjects)
{
    MainWidget::load();
    mIdObjects = aIdObjects;
    ResponseType vResponse = execQuery(
                QString("SELECT id, number, fio, number_passport, address, seria_passport, date_order, bithday, issued_by, when_issued FROM passport WHERE object_fk = %1")
                .arg(mIdObjects));

    if (vResponse.count())
    {
        ResponseRecordType vQuery = vResponse[0];

        mId = vQuery["id"].toInt();
        ui->mpAddress->setText(vQuery["address"].toString());
        ui->mpBithday->setDate(vQuery["bithday"].toDate());
        ui->mpDate->setDate(vQuery["date_order"].toDate());
        ui->mpFIO->setText(vQuery["address"].toString());
        ui->mpIssued->setText(vQuery["issued_by"].toString());
        ui->mpIssueWhen->setDate(vQuery["when_issued"].toDate());
        ui->mpNumber->setText(vQuery["number"].toString());
        ui->mpSeriaPassport->setText(vQuery["seria_passport"].toString());
        ui->mpNumberPassport->setText(vQuery["number_passport"].toString());
        MainWidget::save();
    }
    else
    {
        mId = -1;
        ui->mpAddress->setText("");
        ui->mpDate->setDate(QDate::currentDate());
        ui->mpFIO->setText("");
        ui->mpIssued->setText("");
        ui->mpNumber->setText("");
        ui->mpSeriaPassport->setText("");
        ui->mpNumberPassport->setText("");
    }

}

void PassportWidget::save()
{
    if (mId < 0)
    {
        execQuery(QString("INSERT INTO passport (object_fk) VALUES (%1)")
                  .arg(mIdObjects));
        mId = execQuery(
                    QString("SELECT id FROM passport WHERE object_fk = %1")
                    .arg(mIdObjects))[0]["id"].toInt();
    }
    execQuery(QString("UPDATE passport SET  number = %1, fio = '%2', number_passport = '%3', address = '%4', seria_passport = '%5', date_order = '%6', bithday = '%7', issued_by = '%8', when_issued = '%9' WHERE id = %10")
              .arg(ui->mpNumber->text().isEmpty() ? "0" : ui->mpNumber->text())
              .arg(ui->mpFIO->text().isEmpty() ? "" : ui->mpFIO->text())
              .arg(ui->mpNumberPassport->text().isEmpty() ? "" : ui->mpNumberPassport->text())
              .arg(ui->mpAddress->text().isEmpty() ? "" : ui->mpAddress->text())
              .arg(ui->mpSeriaPassport->text().isEmpty() ? "" : ui->mpSeriaPassport->text())
              .arg(ui->mpDate->date().toString(DATESAVEFORMAT))
              .arg(ui->mpBithday->date().toString(DATESAVEFORMAT))
              .arg(ui->mpIssued->text().isEmpty() ? "" : ui->mpIssued->text())
              .arg(ui->mpIssueWhen->date().toString(DATESAVEFORMAT))
              .arg(mId));
    MainWidget::save();
    back();
}
