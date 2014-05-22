#ifndef ADMINWIDGET_H
#define ADMINWIDGET_H

#include <QWidget>
#include "widgetforcontrol.h"
#include "viewobjects.h"
namespace Ui {
class AdminWidget;
}

class AdminWidget : public WidgetForControl
{
    Q_OBJECT
    
public:
    explicit AdminWidget(QWidget *parent = 0);
    ~AdminWidget();
    virtual QString name();
    virtual QSize minSize();
    virtual QSize maxSize();
    virtual QSize size();
    virtual QString idName();
private slots:
    void on_mpFind_clicked();
    virtual void reload(WidgetForControl*apControl);
    void changeObject(ViewObjects::TypeObject);
    void loadAgents();
    void changeSelect(bool aState);
    void on_mpAddAgent_clicked();

    void on_mpChangeAgent_clicked();

    void on_mpTake_clicked();
    void on_mpRemove_clicked();
private:
    Ui::AdminWidget *ui;
    ViewObjects* mpViewObjects;
};

#endif // ADMINWIDGET_H
