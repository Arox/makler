#include "dialogaside.h"
#include "ui_dialogaside.h"

#include "globalsbase.h"
#include "globals.h"

DialogAside::DialogAside(int aIdObject, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAside),
    mIdObject(aIdObject)
{
    ui->setupUi(this);
    ui->mpButtonAside->setProperty("color", "true");
    ui->mpDate->setDate(QDate::currentDate().addDays(1));
}

DialogAside::~DialogAside()
{
    delete ui;
}

void DialogAside::on_mpButtonAside_clicked()
{
    execQuery(QString("UPDATE objects SET status = 'A' WHERE id = %1")
              .arg(mIdObject));
    execQuery(QString("INSERT INTO aside (id, date_dead, comment) VALUES(%1, '%2', '%3')")
              .arg(mIdObject)
              .arg(ui->mpDate->date().toString(DATESAVEFORMAT))
              .arg(ui->mpEditor->toPlainText()));
    accept();
}

void DialogAside::on_mpButtonCancel_clicked()
{
    reject();
}
