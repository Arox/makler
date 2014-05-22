#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QStack>
#include <QList>

#include "adminwidget.h"
#include "viewbases.h"
#include "addresseditor.h"
#include "widgetforcontrol.h"
#include "editorsettingsdatabase.h"
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
/*private:
    void connectWidget(WidgetForControl* apWidget);
    void disconnectWidget(WidgetForControl* apWidget);
    void insertWidget(int aIndex, WidgetForControl*apWidget);
    WidgetForControl* widget(int aIndex);
    bool removeWidget(int aIndex);
    void appendWidget(WidgetForControl* apWidget);
private slots:
    void backClicked();
    void load(WidgetForControl::SignalWidgetType apControl);
    void backWidget(WidgetForControl* apSender);
    void reloadMyWidget(WidgetForControl* apSender);
    void reloadNameWidget(WidgetForControl*apSender, QString aName);
signals:
    void reloadWidget(WidgetForControl* apControl);*/
private slots:
    void backClicked();
private:
    Ui::SettingsWidget *ui;
    AdminWidget* mpAdmin;
    AddressEditor* mpAddressEditor;
    ViewBases* mpSync;
    EditorSettingsDatabase mEditorDatabase;

    //QList<QStack <WidgetForControl*>> mListWidgets;
};

#endif // SETTINGSWIDGET_H
