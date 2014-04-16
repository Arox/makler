#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "topmenu.h"
#include "ui_topmenu.h"

#include "apartmentwidget.h"
#include "rentwidget.h"
#include "homewidget.h"
#include "findobjectwidget.h"
#include "clientswidget.h"
#include "adminwidget.h"
#include "messagewidget.h"
#include "viewobjectwidget.h"

TopMenu::TopMenu(int aUser_fk, QStringList aRoles, QWidget *parent) :
    ui(new Ui::TopMenu),
    mUser_fk(aUser_fk),
    mpStackWidget(0),
    WidgetForControl(parent)
{
    ui->setupUi(this);
    mpStackWidget = new QStackedWidget(this);

    QList<WidgetForControl*> vWidgets;
    vWidgets.append(new ClientsWidget(mUser_fk,this));

    TableModelApartment* vpModelApartment = new TableModelApartment(this);
    vpModelApartment->addFilter(QString("agent_fk = %1").arg(mUser_fk));
    vWidgets.append(new ViewObjectWidget(vpModelApartment, new ApartmentWidget(mUser_fk), this));

    TableModelRent* vpModelRent = new TableModelRent(this);
    vpModelRent->addFilter(QString("agent_fk = %1").arg(mUser_fk));
    vWidgets.append(new ViewObjectWidget(vpModelRent, new RentWidget(mUser_fk), this));

    TableModelHome* vpModelHome = new TableModelHome(this);
    vpModelHome->addFilter(QString("agent_fk = %1").arg(mUser_fk));
    vWidgets.append(new ViewObjectWidget(vpModelHome, new HomeWidget(mUser_fk), this));

    vWidgets.append(new FindObjectWidget(mUser_fk,this));

    vWidgets.append(new MessageWidget(mUser_fk, this));

    if (aRoles.contains("admin"))
    {
        mIsAdmin = true;
    }
    else
    {
        mIsAdmin = false;
    }

    for (int i = 0; i < vWidgets.count(); ++i)
    {
        QStack<WidgetForControl*> vStack;
        vStack.push(vWidgets.at(i));
        mWidgets.append(vStack);
    }

    inizialize();

    ui->verticalLayout->addWidget(mpStackWidget);

    connect(&mMapper, SIGNAL(mapped(int)), this, SLOT(menuActivated(int)));
}

void TopMenu::inizialize()
{
    for(int i= 0; i <mWidgets.count(); ++i) {
        WidgetForControl* vpWidget = mWidgets.at(i).top();
        ButtonTopMenuWidget* vpMenuElement = new ButtonTopMenuWidget(vpWidget->name(), this);
        vpMenuElement->setObjectName(vpWidget->idName());
        vpMenuElement->setProperty("mainmenu", true);
        ui->menuLayout->insertWidget(i+1, vpMenuElement);

        mpStackWidget->addWidget(vpWidget);
        connectWidget(vpWidget);
        connect(vpMenuElement, SIGNAL(activated()), &mMapper, SLOT(map()));
        mMapper.setMapping(vpMenuElement, i);
        mpButtons.append(vpMenuElement);
    }

    if (mpButtons.count())
    {
        mpButtons[0]->setActive(true);
    }

    ButtonTopMenuWidget* vpMenuElement = new ButtonTopMenuWidget(TRANSLATE("Выход"), this);
    vpMenuElement->setObjectName("exit");
    vpMenuElement->setProperty("mainmenu", true);
    ui->menuLayout->insertWidget(mWidgets.count()+1, vpMenuElement);
    mpButtons.append(vpMenuElement);
    connect(vpMenuElement, SIGNAL(activated()), this, SLOT(exit()));
}

TopMenu::~TopMenu()
{
    delete ui;
}

QString TopMenu::name()
{
    return TRANSLATE("");
}

QSize TopMenu::minSize()
{
    return QSize((mIsAdmin ? 600 : 300) , mIsAdmin ? 180 : 250);
}

QSize TopMenu::maxSize()
{
    return QSize((mIsAdmin ? 600 : 300), mIsAdmin ? 180 : 250);
}

QSize TopMenu::size()
{
    return QSize((mIsAdmin ? 600 : 300), mIsAdmin ? 180 : 250);
}

QString TopMenu::idName()
{
    return "mainmenu";
}

void TopMenu::menuActivated(int aIndex)
{
    for (int i =0; i < mpButtons.count(); ++i)
    {
        mpButtons[i]->setActive(i == aIndex);
    }
    mpStackWidget->setCurrentIndex(aIndex);
}

void TopMenu::exit()
{
    if (window()) window()->close();
    mpButtons.last()->setActive(false);
}

//pair - sender, argument
void TopMenu::load(SignalWidgetType apControl)
{
    WidgetForControl* vpSender = apControl.first;
    WidgetForControl* vpArg = apControl.second;
    Q_ASSERT(vpArg != NULL);

    int vNumber = numWidget(vpSender);
    Q_ASSERT(vNumber >= 0);

    WidgetForControl* vpWidget = mWidgets.at(vNumber).top();
    disconnectWidget(vpWidget);
    mpStackWidget->removeWidget(vpWidget);
    vpWidget->setParent(0);

    mWidgets[vNumber].push(vpArg);
    connectWidget(vpArg);
    mpStackWidget->insertWidget(vNumber, vpArg);
    mpStackWidget->setCurrentIndex(vNumber);
}

void TopMenu::backWidget(WidgetForControl* apSender)
{
    int vNumber = numWidget(apSender);
    Q_ASSERT(vNumber >= 0);

    WidgetForControl* vpWidget = mWidgets.at(vNumber).top();

    disconnectWidget(vpWidget);
    mWidgets[vNumber].pop();
    vpWidget->setParent(0);
    mpStackWidget->removeWidget(vpWidget);

    WidgetForControl* vpParentWidget = mWidgets.at(vNumber).top();
    connectWidget(vpParentWidget);
    vpParentWidget->reload(vpWidget);
    //delete vpWidget;

    mpStackWidget->insertWidget(vNumber, vpParentWidget);
    mpStackWidget->setCurrentIndex(vNumber);
}

void TopMenu::disconnectWidget(WidgetForControl* apWidget)
{
    disconnect(apWidget, SIGNAL(changeWidget(SignalWidgetType)), this, SLOT(load(SignalWidgetType)));
    disconnect(apWidget, SIGNAL(back(WidgetForControl*)), this, SLOT(backWidget(WidgetForControl*)));
}

void TopMenu::connectWidget(WidgetForControl* apWidget)
{
    connect(apWidget, SIGNAL(changeWidget(SignalWidgetType)), this, SLOT(load(SignalWidgetType)));
    connect(apWidget, SIGNAL(back(WidgetForControl*)), this, SLOT(backWidget(WidgetForControl*)));
}

int TopMenu::numWidget(WidgetForControl* apWidget)
{
    for (int i =0; i < mWidgets.count(); ++i)
    {
       if (mWidgets.at(i).top() == apWidget) return i;
    }
    return -1;
}
