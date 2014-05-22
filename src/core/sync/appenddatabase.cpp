#include "appenddatabase.h"
#include "ui_appenddatabase.h"
#include "globalsbase.h"

AppendDatabase::AppendDatabase(QWidget *parent) :
    MainWidget(parent),
    ui(new Ui::AppendDatabase)
{
    ui->setupUi(this);
}

AppendDatabase::~AppendDatabase()
{
    delete ui;
}

void AppendDatabase::on_mpButtons_accepted()
{
    execQuery(QString("INSERT INTO bases (host, name, port, \"user\", pass) VALUES ('%1', '%2', %3, '%4', '%5')")
              .arg(ui->mpHost->text())
              .arg(ui->mpName->text())
              .arg(ui->mpPort->text())
              .arg(ui->mpUser->text())
              .arg(ui->mpPassword->text()));
    emit back();
}

void AppendDatabase::on_mpButtons_rejected()
{
    emit back();
}
