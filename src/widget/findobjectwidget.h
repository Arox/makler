#ifndef FINDOBJECTWIDGET_H
#define FINDOBJECTWIDGET_H

#include <QWidget>
#include "widgetforcontrol.h"
#include <QModelIndex>

namespace Ui {
class FindObjectWidget;
}

class FindObjectWidget : public WidgetForControl
{
    Q_OBJECT
    
public:
    explicit FindObjectWidget(int aUser_fk, QWidget *parent = 0);
    ~FindObjectWidget();
    virtual QString name();
    virtual QSize minSize();
    virtual QSize maxSize();
    virtual QSize size();
    virtual QString idName();
public slots:
    virtual void reload(WidgetForControl*);
private slots:
    void on_mpApartment_clicked();

    void on_mpHome_clicked();

    void on_mpRent_clicked();

    void on_mpView_doubleClicked(const QModelIndex &index);

private:
    Ui::FindObjectWidget *ui;
    int mUser_fk;
};

#endif // FINDOBJECTWIDGET_H
