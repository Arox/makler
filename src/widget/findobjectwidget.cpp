#include "findobjectwidget.h"
#include "ui_findobjectwidget.h"

#include "findapartment.h"
#include "findrent.h"
#include "findhome.h"
#include "tablemodelapartment.h"
#include "tablemodelrent.h"
#include "tablemodelhome.h"
#include "apartmentwidget.h"
#include "rentwidget.h"
#include "homewidget.h"
#include "language.h"

FindObjectWidget::FindObjectWidget(int aUser_fk, QWidget *parent) :
    WidgetForControl(parent),
    ui(new Ui::FindObjectWidget),
    mUser_fk(aUser_fk)
{
    ui->setupUi(this);
    ui->mpView->setVisible(false);
}

FindObjectWidget::~FindObjectWidget()
{
    delete ui;
}

void FindObjectWidget::on_mpApartment_clicked()
{
    emit changeWidget(SignalWidgetType(this, new FindApartment()));
}

void FindObjectWidget::reload(WidgetForControl* apControlFind)
{
    if (dynamic_cast<ApartmentWidget*>(apControlFind)
            || dynamic_cast<RentWidget*>(apControlFind)
            || dynamic_cast<HomeWidget*>(apControlFind))
    {
        TableModel* vpModel = dynamic_cast<TableModel*>(ui->mpView->model());
        if (vpModel)
        {
            ui->mpView->setModel(0);
            vpModel->load();
            ui->mpView->setModel(vpModel);

        }
        delete apControlFind;
        return;
    }
    FindApartment* vpFindApartment = dynamic_cast<FindApartment*>(apControlFind);
    FindRent * vpFindRent = dynamic_cast<FindRent*>(apControlFind);
    FindHome * vpFindHome = dynamic_cast<FindHome*>(apControlFind);
    TableModel* vpModel = 0;
    if (ui->mpView->model())
    {
        delete ui->mpView->model();
    }
    if (vpFindApartment)
    {
        vpModel = new TableModelApartment();
        if (!vpFindApartment->sql().isEmpty())
        {
            if (vpFindApartment->sql() != " ")
            ((TableModelApartment*)vpModel)->addFilter(vpFindApartment->sql());
        }
        else
        {
            return;
        }
    }
    if (vpFindRent)
    {
        vpModel = new TableModelRent();
        if (!vpFindRent->sql().isEmpty())
        {
            if (vpFindRent->sql() != " ")
            ((TableModelRent*)vpModel)->addFilter(vpFindRent->sql());
        }
        else
        {
            return;
        }
    }
    if (vpFindHome)
    {
        vpModel = new TableModelHome();
        if (!vpFindHome->sql().isEmpty())
        {
            if (vpFindHome->sql() != " ")
            ((TableModelHome*)vpModel)->addFilter(vpFindHome->sql());
        }
        else
        {
            return;
        }
    }
    vpModel->setActive(true);
    vpModel->setArchive(false);
    vpModel->setAside(false);
    vpModel->load();
    ui->mpView->setModel(vpModel);
    if (!ui->mpView->isVisible())
    {
        ui->mpView->setVisible(true);
        if (ui->verticalSpacer)
        {
            ui->verticalLayout->removeItem(ui->verticalSpacer);
            delete ui->verticalSpacer;
            ui->verticalSpacer = 0;
        }
    }
    ui->mpView->reset();
    ui->mpView->resizeRowsToContents();
    ui->mpView->resizeColumnsToContents();
    delete apControlFind;
}

void FindObjectWidget::on_mpHome_clicked()
{
    emit changeWidget(SignalWidgetType(this, new FindHome()));
}

void FindObjectWidget::on_mpRent_clicked()
{
    emit changeWidget(SignalWidgetType(this, new FindRent()));
}

void FindObjectWidget::on_mpView_doubleClicked(const QModelIndex &index)
{
    TableModelHome* vpModelHome = dynamic_cast<TableModelHome*>(ui->mpView->model());
    TableModelRent* vpModelRent = dynamic_cast<TableModelRent*>(ui->mpView->model());
    TableModelApartment* vpModelApartment = dynamic_cast<TableModelApartment*>(ui->mpView->model());
    GeneralWidget* vpWidget = 0;
    if (vpModelApartment)
    {
        vpWidget = new ApartmentWidget(mUser_fk);
    }
    if (vpModelRent)
    {
        vpWidget = new RentWidget(mUser_fk);
    }
    if (vpModelHome)
    {
        vpWidget = new HomeWidget(mUser_fk);
    }
    if (vpWidget)
    {
        vpWidget->load(((TableModel*)ui->mpView->model())->id(index.row()));
        emit changeWidget(SignalWidgetType(this, vpWidget));
    }
}

QString FindObjectWidget::name()
{
    return TRANSLATE("Поиск объекта");
}

QSize FindObjectWidget::minSize()
{
    return QSize(0, 0);
}

QSize FindObjectWidget::size()
{
    return QSize(880, 364);
}

QSize FindObjectWidget::maxSize()
{
    return QSize(0, 0);
}

QString FindObjectWidget::idName()
{
    return "find";
}
