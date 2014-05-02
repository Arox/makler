#ifndef TOPMENU_H
#define TOPMENU_H

#include <QWidget>
#include <QList>
#include <QStackedWidget>
#include <QSignalMapper>
#include <QVector>
#include <QStack>
#include <QPair>

#include "widgetforcontrol.h"
#include "buttontopmenuwidget.h"

namespace Ui {
class TopMenu;
}

class TopMenu : public WidgetForControl
{
    Q_OBJECT
    typedef QStack<WidgetForControl*> WidgetsList;
public:
    explicit TopMenu(int aUser_fk, QStringList aRoles, QWidget *parent = 0);
    ~TopMenu();

    void inizialize();

    virtual QString name();
    virtual QSize minSize();
    virtual QSize maxSize();
    virtual QSize size();
    virtual QString idName();
private slots:
    void menuActivated(int aIndex);
    void exit();

//from mainwindow
public slots:
    void load(WidgetForControl::SignalWidgetType apControl);
    void backWidget(WidgetForControl* apSender);
    void reloadMyWidget(WidgetForControl* apSender);
signals:
    void reloadWidget(WidgetForControl* apControl);
protected:
    void disconnectWidget(WidgetForControl *apWidget);
    void connectWidget(WidgetForControl* apWidget);
private:
    int numWidget(WidgetForControl* apWidget);
private:
    Ui::TopMenu *ui;
    //QList<WidgetForControl*> mWidgets;
    int mUser_fk;
    bool mIsAdmin;
    QStackedWidget* mpStackWidget;
    QSignalMapper mMapper;
    QVector<WidgetsList> mWidgets;
    QVector<ButtonTopMenuWidget*> mpButtons;
};

#endif // TOPMENU_H
