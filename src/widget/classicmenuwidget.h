#ifndef CLASSICMENUWIDGET_H
#define CLASSICMENUWIDGET_H

#include <QWidget>
#include "widgetforcontrol.h"
#include "findobjectwidget.h"

namespace Ui {
class ClassicMenuWidget;
}

class ClassicMenuWidget : public WidgetForControl
{
    Q_OBJECT
    
public:
    explicit ClassicMenuWidget(int aUser_fk, QStringList aRoles, QWidget *parent = 0);
    ~ClassicMenuWidget();
    virtual QString name();
    virtual QSize minSize();
    virtual QSize maxSize();
    virtual QSize size();
private slots:
    void on_mpApartment_clicked();
    void on_mpRent_clicked();
    void on_mpHome_clicked();
    void on_mpFindObject_clicked();

    void on_mpClients_clicked();

    void on_mpAdmin_clicked();

    void on_mpMessages_clicked();

    void on_mpExit_clicked();

    void on_mpAddressEditor_clicked();

    void on_mpSync_clicked();

private:
    Ui::ClassicMenuWidget *ui;
    int mUser_fk;
    bool mIsAdmin;
};

#endif // CLASSICMENUWIDGET_H
