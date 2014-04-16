#include "widgetforlist.h"
#include "ui_widgetforlist.h"

#include <QPushButton>
#include <QLabel>
#include <QSpacerItem>

WidgetForList::WidgetForList(QString aName, QVariant aData, bool aNeedRemove, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetForList),
    mData(aData)
{
    ui->setupUi(this);
    //ui->mainLayout->addWidget(new QLabel(aName, this));
    QPushButton* vpLabelButton = new QPushButton(aName, this);
    vpLabelButton->setIconSize(QSize(10, 10));
    vpLabelButton->setFlat(true);
    vpLabelButton->setCursor(QCursor(Qt::PointingHandCursor));
    //vpButton->setMaximumSize(12, vpButton->maximumSize().height());
    ui->mainLayout->addWidget(vpLabelButton);
    connect(vpLabelButton, SIGNAL(clicked()), this, SIGNAL(clickLink()));
    if (aNeedRemove)
    {
        QPushButton* vpButton = new QPushButton(QIcon(":/message/delete.png"), "", this);
        vpButton->setIconSize(QSize(10, 10));
        vpButton->setFlat(true);
        vpButton->setCursor(QCursor(Qt::PointingHandCursor));
        //vpButton->setMaximumSize(12, vpButton->maximumSize().height());
        ui->mainLayout->addWidget(vpButton);
        connect(vpButton, SIGNAL(clicked()), this, SIGNAL(remove()));
    }
    ui->mainLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    setMaximumSize(maximumSize().width(), 20);
}

WidgetForList::~WidgetForList()
{
    delete ui;
}

QVariant WidgetForList::data()
{
    return mData;
}
