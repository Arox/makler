#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "topmenu.h"
#include "ui_topmenu.h"

#include "findobjectwidget.h"
#include "clientswidget.h"
#include "adminwidget.h"
#include "messagewidget.h"
#include "viewobjectwidget.h"
#include "makler.h"
#include "language.h"

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
    vWidgets.append(new ViewObjectWidget(APARTMENT, mUser_fk, this));
    vWidgets.append(new ViewObjectWidget(HOME, mUser_fk, this));
    vWidgets.append(new ViewObjectWidget(RENT, mUser_fk, this));
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
    setObjectName("topmenu");
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

    mpStackWidget->insertWidget(vNumber, vpParentWidget);
    mpStackWidget->setCurrentIndex(vNumber);
}

void TopMenu::reloadMyWidget(WidgetForControl* apSender)
{
    int vNumber = numWidget(apSender);
    Q_ASSERT(vNumber >= 0);

    int vCurrentIndex = mpStackWidget->currentIndex();

    WidgetForControl* vpWidget = mWidgets.at(vNumber).top();

    vpWidget->setParent(0);
    mpStackWidget->removeWidget(vpWidget);

    vNumber = numWidget(apSender);
    mpStackWidget->insertWidget(vNumber, vpWidget);
    vNumber = numWidget(apSender);
    mpStackWidget->setCurrentIndex(vCurrentIndex);
}

void TopMenu::reloadNameWidget(WidgetForControl* apSender, QString aName)
{
    int vNumber = numWidget(apSender);
    Q_ASSERT(vNumber >= 0);
    mpButtons[vNumber]->setText(aName);
}

void TopMenu::disconnectWidget(WidgetForControl* apWidget)
{
    disconnect(apWidget, SIGNAL(changeWidget(WidgetForControl::SignalWidgetType)), this, SLOT(load(WidgetForControl::SignalWidgetType)));
    disconnect(apWidget, SIGNAL(back(WidgetForControl*)), this, SLOT(backWidget(WidgetForControl*)));
    disconnect(apWidget, SIGNAL(reloadMe(WidgetForControl*)), this, SLOT(reloadMyWidget(WidgetForControl*)));
    disconnect(apWidget, SIGNAL(changeName(WidgetForControl*,QString)), this, SLOT(reloadNameWidget(WidgetForControl*,QString)));
}

void TopMenu::connectWidget(WidgetForControl* apWidget)
{
    connect(apWidget, SIGNAL(changeWidget(WidgetForControl::SignalWidgetType)), this, SLOT(load(WidgetForControl::SignalWidgetType)));
    connect(apWidget, SIGNAL(back(WidgetForControl*)), this, SLOT(backWidget(WidgetForControl*)));
    connect(apWidget, SIGNAL(reloadMe(WidgetForControl*)), this, SLOT(reloadMyWidget(WidgetForControl*)));
    connect(apWidget, SIGNAL(changeName(WidgetForControl*,QString)), this, SLOT(reloadNameWidget(WidgetForControl*,QString)));
}

int TopMenu::numWidget(WidgetForControl* apWidget)
{
    for (int i =0; i < mWidgets.count(); ++i)
    {
       if (mWidgets.at(i).top() == apWidget) return i;
    }
    return -1;
}
