#ifndef FINDOBJECTSWIDGET_H
#define FINDOBJECTSWIDGET_H

#include <QWidget>
#include "tablemodelapartmentclient.h"
#include "tablemodelclient.h"
#include "tablemodelhomeclient.h"
#include "tablemodelrentclient.h"
#include "mainwidget.h"

namespace Ui {
class FindObjectsWidget;
}

class FindObjectsWidget : public MainWidget
{
    Q_OBJECT
    
public:
    enum TypeObject
    {
        APARTMENT = 1,
        HOME      = 2,
        RENT      = 3,
        UNKNOW
    };

    explicit FindObjectsWidget(QWidget *parent = 0);
    ~FindObjectsWidget();
    void setObjectId(int aObjectId);
    FindObjectsWidget::TypeObject currentType();
public slots:
    void load(FindObjectsWidget::TypeObject aObject, QString aFilter);
signals:
    void find(FindObjectsWidget::TypeObject aObject);
    void activateObject(FindObjectsWidget::TypeObject aType, int aId);
protected:
    void reloadView();
private slots:
    void on_mpApartment_clicked();

    void on_mpHome_clicked();

    void on_mpRent_clicked();

    void on_mpFind_clicked();

    void checkedView();
    void on_mpRemove_clicked();

    void on_mpClear_clicked();

    void on_mpButtonView_clicked();

    void on_mpView_doubleClicked(const QModelIndex &index);
private:
    Ui::FindObjectsWidget *ui;
    TableModelApartmentClient mApartmentModel;
    TableModelRentClient mRentModel;
    TableModelHomeClient mHomeModel;
};

#endif // FINDOBJECTSWIDGET_H
