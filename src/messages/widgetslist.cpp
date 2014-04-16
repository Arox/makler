#include "widgetslist.h"
#include "ui_widgetslist.h"

const int WidgetsList::gCountButtonsUser = 3;

WidgetsList::WidgetsList(bool aNeedRemove, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetsList),
    mNeedRemove(aNeedRemove)
{
    ui->setupUi(this);
    connect(&mMapper, SIGNAL(mapped(int)), this, SLOT(removeElement(int)));
    connect(&mClickMapper, SIGNAL(mapped(int)), this, SLOT(clickLink(int)));
}

WidgetsList::~WidgetsList()
{
    delete ui;
}

void WidgetsList::add(QString aName, QVariant aData)
{
    WidgetForList* vpButton = new WidgetForList(aName, aData, mNeedRemove, this);
    mWidgetsList.append(vpButton);
    int vIndex = mWidgetsList.count() - 1;
    ui->mainLayout->insertWidget(0, vpButton/*, vIndex / gCountButtonsUser, vIndex % gCountButtonsUser*/);
    connect(vpButton, SIGNAL(remove()), &mMapper, SLOT(map()));
    connect(vpButton, SIGNAL(clickLink()), &mClickMapper, SLOT(map()));
    mMapper.setMapping(vpButton, vIndex);
    mClickMapper.setMapping(vpButton, vIndex);
}

QList<QVariant> WidgetsList::data()
{
    QList<QVariant> mDatas;
    foreach (WidgetForList* vpWidget, mWidgetsList)
    {
        mDatas.append(vpWidget->data());
    }
    return mDatas;
}

void WidgetsList::removeElement(int aIndex)
{
    WidgetForList* vpButton = mWidgetsList.at(aIndex);
    mWidgetsList.removeAt(aIndex);
    delete vpButton;
    for (int i = aIndex; i < mWidgetsList.count(); ++i)
    {
        WidgetForList* vpNextButton = mWidgetsList.at(i);
        mMapper.removeMappings(vpNextButton);
        mMapper.setMapping(vpNextButton, i);
    }
    reloadButtonsUser(aIndex);
    emit removeElement();
}

void WidgetsList::clickLink(int aIndex)
{
    WidgetForList* vpButton = mWidgetsList.at(aIndex);
    emit clickLink(vpButton->data().toByteArray());
}

void WidgetsList::reloadButtonsUser(int aStartIndex)
{
    for (int i = aStartIndex; i < mWidgetsList.count(); ++i)
    {
        WidgetForList* vpNextButton = mWidgetsList.at(i);
        ui->mainLayout->insertWidget(0, vpNextButton/*, i / gCountButtonsUser, i % gCountButtonsUser*/);
    }
}

QVariant WidgetsList::data(int index)
{
    return mWidgetsList.at(index)->data();
}

int WidgetsList::countData()
{
    return mWidgetsList.count();
}

void WidgetsList::setNeedRemove(bool aNeedRemove)
{
    mNeedRemove = aNeedRemove;
}
