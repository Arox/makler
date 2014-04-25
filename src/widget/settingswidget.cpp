#include "settingswidget.h"
#include "ui_settingswidget.h"
#include "globalsbase.h"
SettingsWidget::SettingsWidget(int aUser_fk, QStringList aRoles, QWidget *parent) :
    WidgetForControl(parent),
    ui(new Ui::SettingsWidget)
  ,mpAdmin(0)
  ,mpAddressEditor(0)
  ,mpSync(0)
{
    Q_UNUSED(aUser_fk);
    ui->setupUi(this);
    while (ui->mpContainer->count()) ui->mpContainer->removeItem(0);
    if (aRoles.contains("admin"))
    {
        mpAdmin = new AdminWidget(this);
        ui->mpContainer->addItem(mpAdmin, TRANSLATE("Администрирование"));

        mpAddressEditor = new AddressEditor(this);
        ui->mpContainer->addItem(mpAddressEditor, TRANSLATE("Добавление адресов"));

        mpSync = new ViewBases(this);
        ui->mpContainer->addItem(mpSync, TRANSLATE("Синхронизация данных"));
    }
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

QString SettingsWidget::name()
{
    return TRANSLATE("Настройки");
}

QSize SettingsWidget::minSize()
{
    return QSize();
}

QSize SettingsWidget::maxSize()
{
    return QSize();
}

QSize SettingsWidget::size()
{
    return QSize();
}

QString SettingsWidget::idName()
{
    return "settings";
}