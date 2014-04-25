#include "adminwidget.h"
#include "ui_adminwidget.h"

#include "findapartment.h"
#include "findhome.h"
#include "findrent.h"

#include "globalsbase.h"

#include "agentwidget.h"
#include "dialoguniversal.h"

AdminWidget::AdminWidget(QWidget *parent) :
    WidgetForControl(parent),
    ui(new Ui::AdminWidget)
{
    ui->setupUi(this);
    mpViewObjects = new ViewObjects(this);
    connect(mpViewObjects, SIGNAL(changeModel(ViewObjects::TypeObject)), this, SLOT(changeObject(ViewObjects::TypeObject)));
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
        mpViewObjects->load(ViewObjects::APARTMENT, vpApartment->sql());
        return;
    }
    if (vpHome)
    {
        mpViewObjects->load(ViewObjects::HOME, vpHome->sql());
        return;
    }
    if (vpRent)
    {
        mpViewObjects->load(ViewObjects::RENT, vpRent->sql());
        return;
    }
}

void AdminWidget::on_mpFind_clicked()
{
    switch(mpViewObjects->type())
    {
    case ViewObjects::APARTMENT:
        emit changeWidget(SignalWidgetType(this, new FindApartment(this)));
        break;
    case ViewObjects::HOME:
        emit changeWidget(SignalWidgetType(this, new FindHome(this)));
        break;
    case ViewObjects::RENT:
        emit changeWidget(SignalWidgetType(this, new FindRent(this)));
        break;
    default:
        break;
    }
}

void AdminWidget::changeObject(ViewObjects::TypeObject aType)
{
    changeSelect(mpViewObjects->isSelected());
    if ((aType == ViewObjects::APARTMENT)
            || (aType == ViewObjects::HOME)
            || (aType == ViewObjects::RENT))
    {
        ui->mpFind->setEnabled(true);
    }
    else
    {
        ui->mpFind->setEnabled(false);
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
    mpViewObjects->changeAgent(ui->mpAgents->itemData(ui->mpAgents->currentIndex()).toInt());
}
