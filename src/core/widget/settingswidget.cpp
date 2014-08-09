#include "settingswidget.h"
#include "ui_settingswidget.h"
#include "language.h"
SettingsWidget::SettingsWidget(int aUser_fk, QStringList aRoles, QWidget *parent) :
    WidgetForControl(parent),
    ui(new Ui::SettingsWidget)
  ,mpAdmin(0)
  ,mpAddressEditor(0)
  ,mpSync(0)
{
    Q_UNUSED(aUser_fk);
    ui->setupUi(this);
    ui->mpBack->setProperty("color", "true");
    while (ui->mpContainer->count()) ui->mpContainer->removeItem(0);

    if (aRoles.contains("admin"))
    {
        mpAdmin = new AdminWidget(this);
        ui->mpContainer->addItem(mpAdmin, TRANSLATE("Администрирование"));

        mpAddressEditor = new AddressEditor(this);
        ui->mpContainer->addItem(mpAddressEditor, TRANSLATE("Добавление адресов"));

        /*mpSync = new ViewBases(this);
        ui->mpContainer->addItem(mpSync, TRANSLATE("Синхронизация данных"));*/
    }
    ui->mpContainer->addItem(&mEditorDatabase, TRANSLATE("Настройки подключения к БД"));
    ui->mpContainer->addItem(&mStyles, TRANSLATE("Стили"));

    connect(ui->mpBack, SIGNAL(clicked()), this, SLOT(backClicked()));
}

void SettingsWidget::backClicked()
{
    emit back(this);
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
/*
void SettingsWidget::load(WidgetForControl::SignalWidgetType apControl)
{

}

void SettingsWidget::backWidget(WidgetForControl* apSender)
{

}

void SettingsWidget::reloadMyWidget(WidgetForControl* apSender)
{

}

void SettingsWidget::reloadNameWidget(WidgetForControl*apSender, QString aName)
{

}

void SettingsWidget::connectWidget(WidgetForControl* apWidget)
{
    connect(apWidget, SIGNAL(changeWidget(WidgetForControl::SignalWidgetType)), this, SLOT(load(WidgetForControl::SignalWidgetType)));
    connect(apWidget, SIGNAL(back(WidgetForControl*)), this, SLOT(backWidget(WidgetForControl*)));
    connect(apWidget, SIGNAL(reloadMe(WidgetForControl*)), this, SLOT(reloadMyWidget(WidgetForControl*)));
    connect(apWidget, SIGNAL(changeName(WidgetForControl*,QString)), this, SLOT(reloadNameWidget(WidgetForControl*,QString)));
}

void SettingsWidget::disconnectWidget(WidgetForControl* apWidget)
{
    disconnect(apWidget, SIGNAL(changeWidget(WidgetForControl::SignalWidgetType)), this, SLOT(load(WidgetForControl::SignalWidgetType)));
    disconnect(apWidget, SIGNAL(back(WidgetForControl*)), this, SLOT(backWidget(WidgetForControl*)));
    disconnect(apWidget, SIGNAL(reloadMe(WidgetForControl*)), this, SLOT(reloadMyWidget(WidgetForControl*)));
    disconnect(apWidget, SIGNAL(changeName(WidgetForControl*,QString)), this, SLOT(reloadNameWidget(WidgetForControl*,QString)));
}

void SettingsWidget::insertWidget(int aIndex, WidgetForControl* apWidget)
{
    if (mListWidgets.count() > i)
    {
        if (mListWidgets[i].top())
        {
            disconnect(mListWidgets[i].top());
        }
        connect(apWidget);
        mListWidgets[i].push(apWidget);
        ui->mpContainer->ite
    }
}

WidgetForControl* SettingsWidget::widget(int aIndex)
{

}

bool SettingsWidget::removeWidget(int aIndex)
{

}

void SettingsWidget::appendWidget(WidgetForControl* apWidget)
{

}
*/
