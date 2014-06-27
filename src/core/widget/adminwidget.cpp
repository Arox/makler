#include "adminwidget.h"
#include "ui_adminwidget.h"

#include "findapartment.h"
#include "findhome.h"
#include "findrent.h"

#include "globalsbase.h"
#include "language.h"

#include "agentwidget.h"
#include "dialoguniversal.h"
#include "dialogforwidgetforcontrol.h"
#include <QMessageBox>

AdminWidget::AdminWidget(QWidget *parent) :
    WidgetForControl(parent),
    ui(new Ui::AdminWidget)
{
    ui->setupUi(this);
    mpViewObjects = new ConsalidatorObjects(this);
    connect(mpViewObjects, SIGNAL(changeModel(ConsalidatorObjects::TypeObject)), this, SLOT(changeObject(ConsalidatorObjects::TypeObject)));
    connect(mpViewObjects, SIGNAL(isSelect(bool)), this, SLOT(changeSelect(bool)));
    ui->mainLayout->insertWidget(0, mpViewObjects);
    loadAgents();
    changeSelect(false);
}

AdminWidget::~AdminWidget()
{
    delete ui;
}

void AdminWidget::loadAgents()
{
    ui->mpAgents->clear();
    ResponseType vResponse = execQuery(QString("SELECT mans.id as id, mans.sername as sername, mans.name as name, mans.patronymic as patronymic FROM users INNER JOIN mans ON users.man_fk = mans.id WHERE users.is_active = TRUE ORDER BY sername, name, patronymic"));
    for (int i = 0; i < vResponse.count(); ++i)
    {
        ResponseRecordType vRecord = vResponse[i];
        ui->mpAgents->addItem(QString("%1 %2 %3")
                .arg(vRecord["sername"].toString())
                .arg(vRecord["name"].toString())
                .arg(vRecord["patronymic"].toString())
                , vRecord["id"].toInt());
    }
}

void AdminWidget::reload(WidgetForControl* apControl)
{
    FindApartment* vpApartment = dynamic_cast<FindApartment*>(apControl);
    FindHome* vpHome = dynamic_cast<FindHome*>(apControl);
    FindRent* vpRent = dynamic_cast<FindRent*>(apControl);
    if (vpApartment)
    {
        mpViewObjects->load(ConsalidatorObjects::APARTMENT, vpApartment->sql());
        return;
    }
    if (vpHome)
    {
        mpViewObjects->load(ConsalidatorObjects::HOME, vpHome->sql());
        return;
    }
    if (vpRent)
    {
        mpViewObjects->load(ConsalidatorObjects::RENT, vpRent->sql());
        return;
    }
}

void AdminWidget::on_mpFind_clicked()
{

    FindWidget* vpFind = 0;
    QString vName;
    ConsalidatorObjects::TypeObject vType;
    switch(mpViewObjects->type())
    {
    case ConsalidatorObjects::APARTMENT:
        vpFind = new FindApartment(this);
        vName = TRANSLATE("Поиск квартиры");
        vType = ConsalidatorObjects::APARTMENT;
        break;
    case ConsalidatorObjects::HOME:
        vpFind = new FindHome(this);
        vName = TRANSLATE("Поиск дома");
        vType = ConsalidatorObjects::HOME;
        break;
    case ConsalidatorObjects::RENT:
        vpFind = new FindRent(this);
        vName = TRANSLATE("Поиск аренды");
        vType = ConsalidatorObjects::RENT;
        break;
    default:
        break;
    }
    if (vpFind)
    {
        DialogForWidgetForControl* vpDialog = new DialogForWidgetForControl(vpFind, vName, this);
        vpDialog->exec();
        mpViewObjects->load(vType, vpFind->sql());
    }
}

void AdminWidget::changeObject(ConsalidatorObjects::TypeObject aType)
{
    changeSelect(mpViewObjects->isSelected());
    if ((aType == ConsalidatorObjects::APARTMENT)
            || (aType == ConsalidatorObjects::HOME)
            || (aType == ConsalidatorObjects::RENT))
    {
        ui->mpFind->setVisible(true);
    }
    else
    {
        ui->mpFind->setVisible(false);
    }
}

void AdminWidget::changeSelect(bool aState)
{
    ui->mpTake->setEnabled(aState);
    ui->mpRemove->setEnabled(aState);
}

void AdminWidget::on_mpAddAgent_clicked()
{
    AgentWidget* vpWidget = new AgentWidget(this);
    DialogUniversal vDialog(vpWidget, TRANSLATE("Создание агента"), this);
    vDialog.exec();

    loadAgents();
}

void AdminWidget::on_mpChangeAgent_clicked()
{
    AgentWidget* vpWidget = new AgentWidget(this);
    vpWidget->load(ui->mpAgents->itemData(ui->mpAgents->currentIndex()).toInt());
    DialogUniversal vDialog(vpWidget, TRANSLATE("Редактирование агента"), this);
    vDialog.exec();
    loadAgents();
}

QString AdminWidget::name()
{
    return TRANSLATE("Администрирование");
}

QSize AdminWidget::minSize()
{
    return QSize(0, 0);
}

QSize AdminWidget::size()
{
    return QSize(1125, 840);
}

QSize AdminWidget::maxSize()
{
    return QSize(0, 0);
}

QString AdminWidget::idName()
{
    return "admin";
}

void AdminWidget::on_mpTake_clicked()
{
    if (QMessageBox::question(this, TRANSLATE("Передача объекта"), QString(TRANSLATE("Вы уверены, что хотите передать объект \"%1\"").arg(ui->mpAgents->currentText()))) == QMessageBox::Yes)
    {
        mpViewObjects->changeAgent(ui->mpAgents->itemData(ui->mpAgents->currentIndex()).toInt());
    }
}

void AdminWidget::on_mpRemove_clicked()
{
    if (mpViewObjects->isSelected())
    {
        if (QMessageBox::question(this, TRANSLATE("Удаление"), TRANSLATE("Вы действительно хотите удалить выделенные объекты?")) == QMessageBox::Yes)
        {
            mpViewObjects->removeSelected();
            mpViewObjects->reload();
        }
    }
}
