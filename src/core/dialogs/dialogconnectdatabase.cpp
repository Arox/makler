#include "dialogconnectdatabase.h"
#include "ui_dialogconnectdatabase.h"
#include "language.h"

#include "messages.h"

DialogConnectDatabase::DialogConnectDatabase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConnectDatabase)
  ,mConnectWidget(this)
{
    ui->setupUi(this);
    ui->verticalLayout->insertWidget(0, &mConnectWidget);
    ui->mpOk->setProperty("color", "true");
    mConnectWidget.readSettings();
}

DialogConnectDatabase::~DialogConnectDatabase()
{
    delete ui;
}

void DialogConnectDatabase::on_mpOk_clicked()
{
    if (mConnectWidget.isConnect())
    {
        mConnectWidget.saveSettings();
        accept();
    }
    else
    {
        warning(this, TRANSLATE("Ошибка подключения"), TRANSLATE("Не удалось подключиться к базе данных"));
    }
}

void DialogConnectDatabase::on_mpExit_clicked()
{
    reject();
}
