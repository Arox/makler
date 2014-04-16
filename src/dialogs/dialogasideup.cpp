#include "dialogasideup.h"
#include "ui_dialogasideup.h"

#include "globals.h"

DialogAsideUp::DialogAsideUp(int aUser_fk, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAsideUp),
    mUser_fk(aUser_fk)
{
    ui->setupUi(this);
    load();
}

DialogAsideUp::~DialogAsideUp()
{
    delete ui;
}

int DialogAsideUp::countAside()
{
    return mData.count();
}

void DialogAsideUp::load()
{
    mData = execQuery("SELECT id, date_dead, comment FROM aside WHERE date_dead <= now()");
    ui->mpListAsides->clear();
    foreach (ResponseRecordType vRecord, mData) {
        ui->mpListAsides->addItem(QString("%1").arg(vRecord["id"].toInt()));
    }
}

void DialogAsideUp::on_mpListAsides_activated(const QModelIndex &index)
{
    Q_UNUSED(index);
}

void DialogAsideUp::on_mpListAsides_itemSelectionChanged()
{
    bool aFlagEnabled = false;
    if (ui->mpListAsides->selectedItems().count())
    {
        aFlagEnabled = true;
    }
    ui->mpButtonAside->setEnabled(aFlagEnabled);
    ui->mpButtonUp->setEnabled(aFlagEnabled);
    ui->mpDate->setEnabled(aFlagEnabled);
    ui->mpEditor->setEnabled(aFlagEnabled);

    if (aFlagEnabled)
    {
        int index = ui->mpListAsides->row(ui->mpListAsides->selectedItems().at(0));
        ui->mpDate->setDate(mData[index]["date_dead"].toDate().addDays(1));
        ui->mpEditor->setPlainText(mData[index]["comment"].toString());
    }
    else
    {
        if(!countAside())
        {
            accept();
        }
    }
}

void DialogAsideUp::on_mpButtonAside_clicked()
{
    if (ui->mpListAsides->selectedItems().count())
    {
        int index = ui->mpListAsides->row(ui->mpListAsides->selectedItems().at(0));
        execQuery(QString("UPDATE aside SET date_dead = '%2', comment = '%3' WHERE id = %1")
                  .arg(mData[index]["id"].toInt())
                .arg(ui->mpDate->date().toString(DATESAVEFORMAT))
                .arg(ui->mpEditor->toPlainText()));
        load();
    }
}

void DialogAsideUp::on_mpButtonUp_clicked()
{
    if (ui->mpListAsides->selectedItems().count())
    {
        int index = ui->mpListAsides->row(ui->mpListAsides->selectedItems().at(0));
        execQuery(QString("DELETE FROM aside WHERE id = %1").arg(mData[index]["id"].toInt()));
        execQuery(QString("UPDATE objects SET status = NULL WHERE id = %1").arg(mData[index]["id"].toInt()));
        load();
    }
}
