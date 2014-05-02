#ifndef VIEWOBJECTWIDGET_H
#define VIEWOBJECTWIDGET_H


#include <QSignalMapper>
#include <QPushButton>

#include "tablemodel.h"
#include "generalwidget.h"
#include "widgetforcontrol.h"
#include "phonefinddialog.h"
#include "findwidget.h"

namespace Ui {
class ViewObjectWidget;
}

class ViewObjectWidget : public WidgetForControl
{
    Q_OBJECT
    
public:
    explicit ViewObjectWidget(TableModel* apModel, GeneralWidget* apWidgetAdd, FindWidget* apWidgetFind, QWidget *parent = 0);
    ~ViewObjectWidget();
    
    virtual QString name();
    virtual QSize minSize();
    virtual QSize maxSize();
    virtual QSize size();
    virtual QString idName();
private slots:
    void on_mpButtonActive_clicked();
    void on_mpButtonDeferred_clicked();
    void on_mpButtonArchive_clicked();
    void on_mpView_doubleClicked(const QModelIndex &index);
    void on_mpButtonAdd_clicked();
    void on_mpButtonAside_clicked();
    void on_mpButtonShoot_clicked();

    void on_mpButtonView_clicked();

protected:
    bool isActive(int aRow);
    void load(const QModelIndex &index = QModelIndex());
    void reloadModel();
private slots:
    void clickFilter();
    void clickClearFilter();
public slots:
    void reloadObject(int aRow);
    void setButton();
    virtual void reload(WidgetForControl*);
signals:
    void select(int);
    void add();
private:
    Ui::ViewObjectWidget *ui;
    QSignalMapper mMapper;
    TableModel* mpModel;
    QList<QPushButton*> mListButtonsTable;
    GeneralWidget* mpWidgetAdd;
    FindWidget* mpWidgetFind;
    QModelIndex mCurrentIndex;
    PhoneFindDialog mDialog;
};

#endif // VIEWOBJECTWIDGET_H
