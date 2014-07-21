#include "locationselect.h"
#include <QCompleter>
LocationSelect::LocationSelect(QWidget *parent) :
    QComboBox(parent)
  , mStatus(location::NORMAL)
{
    setEditable(true);
    setInsertPolicy(QComboBox::NoInsert);
    connect(this, SIGNAL(changeType(int)), this, SLOT(on_changeStatus(int)));
}

void LocationSelect::setType(int aType)
{
    mStatus = aType;
    emit changeType(mStatus);
}

bool LocationSelect::isType(location::TypeSelect aType)
{
    return mStatus & aType;
}

void LocationSelect::on_changeStatus(int aType)
{
    if (isType(location::MULTISELECT))
    {
        connect(this, SIGNAL(activated(int)), this, SLOT(clickItemComboBox(int)));
    }
    else
    {
        disconnect(this, SIGNAL(activated(int)), this, SLOT(clickItemComboBox(int)));
    }
    if (modelSelect()) modelSelect()->setType(aType);
    if (modelCompleter()) modelCompleter()->setType(aType);
    reload();
}

void LocationSelect::load()
{
    if (model()) delete model();
    if (modelSelect())
    {
        setModel(modelSelect());
    }

    if (completer()) delete completer();
    if (modelCompleter())
    {
        QCompleter* vpCompleter = new QCompleter(modelCompleter(), this);
        vpCompleter->setCaseSensitivity(Qt::CaseInsensitive);
        vpCompleter->setFilterMode(Qt::MatchContains);
        setCompleter(vpCompleter);
    }

    if (isType(location::NORMAL) || isType(location::MULTISELECT))
    {
        setCurrentIndex(0);
    }
}

void LocationSelect::reload()
{
    if (completer())
    {
        completer()->setModel(modelCompleter());
    }
    if (modelSelect()) setModel(modelSelect());
}

QList<int> LocationSelect::idComboBox()
{
    QList<int> vResult;
    if (currentIndex() >= 0)
    {
        int vItem = itemData(currentIndex()).toInt();
        if (vItem >= 0)
        {
            vResult.append(vItem);
        }
    }
    return vResult;
}

QList<int> LocationSelect::idsComboBox()
{
    QList<int> vResult;
    for (int i = 0; i < count(); ++i)
    {
        if (!itemIcon(i).isNull())
        {
            vResult.append(itemData(i).toInt());
        }
    }
    return vResult;
}

void LocationSelect::clickItemComboBox(int aIndex)
{
    if (count() > aIndex && aIndex >= 0)
    {
        if (itemIcon(aIndex).isNull())
        {
            setItemIcon(aIndex, QIcon(":/select/picture/ok.png"));
            setCurrentIndex(aIndex);
        }
        else
        {
            setItemIcon(aIndex, QIcon());
        }
    }
}

QList<int> LocationSelect::values()
{
    if (isType(location::MULTISELECT))
    {
        return idsComboBox();
    }
    return  idComboBox();
}
