#ifndef CLIENTSWIDGET_H
#define CLIENTSWIDGET_H

#include <QWidget>
#include <QSignalMapper>
#include <QPushButton>
#include <QComboBox>
#include <QList>
#include <QPair>

#include "tablemodelclient.h"
#include "generalwidget.h"
#include "widgetforcontrol.h"
#include "findobjectswidget.h"
#include "clientheaderalternativewidget.h"

namespace Ui {
class ClientsWidget;
}

class ClientsWidget : public WidgetForControl
{
    Q_OBJECT

public:
    explicit ClientsWidget(int aAgent, QWidget *parent = 0);
    ~ClientsWidget();
    virtual QString name();
    virtual QSize minSize();
    virtual QSize maxSize();
    virtual QSize size();
    virtual QString idName();
public slots:
    void add();
    void reloadObject(int aRow);
    void setButton();
    void setComboBox();
    void loadView();
    void reload(WidgetForControl* apControlFind);
    void changeTypeObject(int aRow);
protected slots:
     void reloadView();
protected:
    bool isActive(int aRow);
    void load(const QModelIndex &index = QModelIndex());

private slots:
    void on_mpButtonAdd_clicked();

    void on_mpButtonAside_clicked();

    void on_mpButtonShoot_clicked();

    void on_mpButtonActive_clicked();

    void on_mpButtonDeferred_clicked();

    void on_mpButtonArchive_clicked();

    void on_mpView_activated(const QModelIndex &index);

    void findObject(FindObjectsWidget::TypeObject aObject);
    void activateObject(FindObjectsWidget::TypeObject aType, int aId);
    void on_mpFilterPhone_textChanged(const QString &arg1);

    void on_mpClearFilter_clicked();

private:
    Ui::ClientsWidget *ui;
    QSignalMapper mMapper;
    QList<QPushButton*> mListButtonsTable;

    QSignalMapper mMapperBoxes;
    QList<QComboBox*> mListBoxesTable;

    TableModelClient mClientModel;
    ClientHeaderAlternativeWidget mClientWidget;
    FindObjectsWidget mFindObject;
    int mAgent;

    QList< QPair<QString, int> >  mValuesTypeObject;
};

#endif // CLIENTSWIDGET_H
