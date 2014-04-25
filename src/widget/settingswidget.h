#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>

#include "adminwidget.h"
#include "viewbases.h"
#include "addresseditor.h"
#include "widgetforcontrol.h"

namespace Ui {
class SettingsWidget;
}

class SettingsWidget : public WidgetForControl
{
    Q_OBJECT

public:
    explicit SettingsWidget(int aUser_fk, QStringList aRoles, QWidget *parent = 0);
    ~SettingsWidget();

    virtual QString name();
    virtual QSize minSize();
    virtual QSize maxSize();
    virtual QSize size();
    virtual QString idName();
private:
    Ui::SettingsWidget *ui;
    AdminWidget* mpAdmin;
    AddressEditor* mpAddressEditor;
    ViewBases* mpSync;
};

#endif // SETTINGSWIDGET_H