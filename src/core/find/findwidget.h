#ifndef FINDWIDGET_H
#define FINDWIDGET_H

#include <QWidget>
#include "widgetforcontrol.h"
namespace Ui {
class FindWidget;
}

class FindWidget : public WidgetForControl
{
    Q_OBJECT

public:
    explicit FindWidget(QWidget *parent = 0);
    ~FindWidget();
    virtual QString sql();
    virtual QString join();
    virtual QString name();
    virtual QSize minSize();
    virtual QSize maxSize();
    virtual QSize size();
    virtual QString idName();
    void clearFind();
private slots:
    void on_mpFind_clicked();
    void on_mpBack_clicked();
protected:
    void setSql(QString);
    void setJoin(QString);
    virtual void getData() = 0;
    virtual void getBack();
    void addWidget(QWidget* apWidget);
private:
    Ui::FindWidget *ui;
    QString mSql;
    QString mJoin;
};

#endif // FINDWIDGET_H
