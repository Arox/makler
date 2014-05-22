#ifndef VIEWOBJECTS_H
#define VIEWOBJECTS_H

#include <QWidget>
#include <QPushButton>

#include "tablemodelapartment.h"
#include "tablemodelclient.h"
#include "tablemodelhome.h"
#include "tablemodelrent.h"
#include "mainwidget.h"

namespace Ui {
class ViewObjects;
}

class ViewObjects : public MainWidget
{
    Q_OBJECT

public:
    enum TypeObject
    {
        APARTMENT = 1,
        HOME      = 2,
        RENT      = 3,
        CLIENT    = 4,
        AGENT     = 5,
        UNDEFINED
    };

    explicit ViewObjects(QWidget *parent = 0);
    ~ViewObjects();
    void load(ViewObjects::TypeObject aObject, QString aFilter);
    ViewObjects::TypeObject type();
    void changeAgent(int aIdAgent);
    bool isSelected();
    void reload();
    void removeSelected();
signals:
    void changeModel(ViewObjects::TypeObject);
    void isSelect(bool);
protected:
    void setCheckStatus(QPushButton* apButton);
    void loadAgents();
private slots:
    void on_mpActive_clicked();

    void on_mpAside_clicked();

    void on_mpArchive_clicked();

    void on_mpApartment_clicked();

    void on_mpRent_clicked();

    void on_mpHome_clicked();

    void on_mpClient_clicked();

    void selectChange();
    void on_mpSelectAll_clicked();

    void on_mpAgents_currentIndexChanged(int index);
private:
    Ui::ViewObjects *ui;
    TableModelApartment mApartmnet;
    TableModelHome mHome;
    TableModelRent mRent;
    TableModelClient mClient;

    QList<TableModel*> mModelsList;
    QString mOldFilter;
};

#endif // VIEWOBJECTS_H
