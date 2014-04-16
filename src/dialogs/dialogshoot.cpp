#include "dialogshoot.h"
#include "ui_dialogshoot.h"

#include "globalsbase.h"

DialogShoot::DialogShoot(int aIdObject, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShoot),
    mIdObject(aIdObject)
{
    ui->setupUi(this);
}

DialogShoot::~DialogShoot()
{
    delete ui;
}

void DialogShoot::on_mpButtonShoot_clicked()
{
    execQuery(QString("UPDATE objects SET status = 'S' WHERE id = %1")
              .arg(mIdObject));
    execQuery(QString("INSERT INTO shoot (id, comment) VALUES(%1, '%2')")
              .arg(mIdObject)
              .arg(ui->mpEditor->toPlainText()));
    accept();
}

void DialogShoot::on_mpButtonCancel_clicked()
{
    reject();
}
