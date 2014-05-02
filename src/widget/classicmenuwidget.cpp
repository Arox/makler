#include "classicmenuwidget.h"
#include "ui_classicmenuwidget.h"

#include "viewobjectwidget.h"
#include "tablemodelapartment.h"
#include "tablemodelhome.h"
#include "tablemodelrent.h"
#include "tablemodelclient.h"
#include "apartmentwidget.h"
#include "rentwidget.h"
#include "homewidget.h"
#include "findobjectwidget.h"
#include "clientswidget.h"
#include "adminwidget.h"
#include "messagewidget.h"
#include "addresseditor.h"
#include "dialoguniversal.h"
#include "viewbases.h"
#include "language.h"

ClassicMenuWidget::ClassicMenuWidget(int aUser_fk, QStringList aRoles, QWidget *parent) :
    WidgetForControl(parent),
    ui(new Ui::ClassicMenuWidget),
    mUser_fk(aUser_fk)
{
    ui->setupUi(this);
    QString vStyle = "height: 80px; font: bold 20px;";
    ui->mpClients->setStyleSheet(vStyle);
    ui->mpApartment->setStyleSheet(vStyle);
    ui->mpExit->setStyleSheet(vStyle);
    ui->mpFindObject->setStyleSheet(vStyle);
    ui->mpHome->setStyleSheet(vStyle);
    ui->mpMessages->setStyleSheet(vStyle);
    ui->mpRent->setStyleSheet(vStyle);
    if (!aRoles.contains("admin"))
    {
        mIsAdmin = false;
        ui->mpAdmin->setVisible(false);
        ui->mpSync->setVisible(false);
        ui->mpAddressEditor->setVisible(false);

        /*ui->verticalLayout->addWidget(ui->mpMessages);
        ui->verticalLayout->addWidget(ui->mpExit);*/
        ui->verticalLayout_2->insertWidget(0, ui->mpFindObject);
    }
    else
    {
        mIsAdmin = true;
        ui->mpAdmin->setStyleSheet(vStyle);
        ui->mpSync->setStyleSheet(vStyle);
        ui->mpAddressEditor->setStyleSheet(vStyle);
    }

}

ClassicMenuWidget::~ClassicMenuWidget()
{
    delete ui;
}

void ClassicMenuWidget::on_mpApartment_clicked()
{
    TableModelApartment* vpModel = new TableModelApartment(this);
    vpModel->addFilter(QString("agent_fk = %1").arg(mUser_fk));
   // emit changeWidget(SignalWidgetType(this, new ViewObjectWidget(vpModel, new ApartmentWidget(mUser_fk), this)));
}

void ClassicMenuWidget::on_mpRent_clicked()
{
    TableModelRent* vpModel = new TableModelRent(this);
    vpModel->addFilter(QString("agent_fk = %1").arg(mUser_fk));
    //emit changeWidget(SignalWidgetType(new ViewObjectWidget(vpModel, new RentWidget(mUser_fk), this)));
}

void ClassicMenuWidget::on_mpHome_clicked()
{
    TableModelHome* vpModel = new TableModelHome(this);
    vpModel->addFilter(QString("agent_fk = %1").arg(mUser_fk));
    //emit changeWidget(SignalWidgetType(new ViewObjectWidget(vpModel, new HomeWidget(mUser_fk), this)));
}

void ClassicMenuWidget::on_mpFindObject_clicked()
{
    //emit changeWidget(SignalWidgetType(new FindObjectWidget(mUser_fk)));
}

void ClassicMenuWidget::on_mpClients_clicked()
{
    //emit changeWidget(SignalWidgetType(new ClientsWidget(mUser_fk, this)));
}


void ClassicMenuWidget::on_mpAdmin_clicked()
{
   // emit changeWidget(SignalWidgetType(new AdminWidget(this)));
}

void ClassicMenuWidget::on_mpMessages_clicked()
{
    //emit changeWidget(new MessageWidget(mUser_fk, this));
}

void ClassicMenuWidget::on_mpAddressEditor_clicked()
{
    DialogUniversal vDialog(new AddressEditor(), TRANSLATE("редактор адресов"));
    vDialog.exec();
}

QString ClassicMenuWidget::name()
{
    return TRANSLATE("");
}

QSize ClassicMenuWidget::minSize()
{
    return QSize((mIsAdmin ? 600 : 300) , mIsAdmin ? 180 : 250);
}

QSize ClassicMenuWidget::maxSize()
{
    return QSize((mIsAdmin ? 600 : 300), mIsAdmin ? 180 : 250);
}

QSize ClassicMenuWidget::size()
{
    return QSize((mIsAdmin ? 600 : 300), mIsAdmin ? 180 : 250);
}

void ClassicMenuWidget::on_mpExit_clicked()
{
    window()->close();
}

void ClassicMenuWidget::on_mpSync_clicked()
{
    //emit changeWidget(new ViewBases(this));
}
