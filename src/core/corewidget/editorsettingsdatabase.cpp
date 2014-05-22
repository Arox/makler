#include "editorsettingsdatabase.h"
#include "ui_editorsettingsdatabase.h"
#include "language.h"

#include <QMessageBox>
EditorSettingsDatabase::EditorSettingsDatabase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorSettingsDatabase)
  ,mConnectWidget(this)
{
    ui->setupUi(this);
    mConnectWidget.readSettings();
    ui->mainLayout->insertWidget(1, &mConnectWidget);

    connect(ui->mpTry, SIGNAL(clicked()), &mConnectWidget, SLOT(showisConnect()));
    connect(ui->mpRestore, SIGNAL(clicked()), &mConnectWidget, SLOT(readSettings()));
}

EditorSettingsDatabase::~EditorSettingsDatabase()
{
    delete ui;
}



void EditorSettingsDatabase::on_mpSave_clicked()
{
    bool vSave = true;
    if (!mConnectWidget.isConnect())
    {
        vSave = false;
        if (QMessageBox::question(this, TRANSLATE("Ошибка подключения"), TRANSLATE("Не удалось подключиться к базе данных. Все равно сохранить данные?")) == QMessageBox::Yes)
        {
            vSave = true;
        }
    }
    if (vSave)
    {
        mConnectWidget.saveSettings();
        QMessageBox::information(this, TRANSLATE("Данные сохранены"), TRANSLATE("Данные успешно сохранены"));
    }
}
