#include "addinfoaboutdatabase.h"
#include "ui_addinfoaboutdatabase.h"

AddInfoAboutDatabase::AddInfoAboutDatabase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddInfoAboutDatabase)
{
    ui->setupUi(this);
}

AddInfoAboutDatabase::~AddInfoAboutDatabase()
{
    delete ui;
}
