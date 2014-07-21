#include "findrent.h"

#include <QStringList>

FindRent::FindRent(QWidget *parent) :
    FindWidget(parent),
    mAddress(location::FIND | location::MULTISELECT)
{
    mInformation.setNameDictionary("rent", 4, true);
    addWidget(&mInformation);
    addWidget(&mArea);
    addWidget(&mType);
    addWidget(&mPrice);
    addWidget(&mAddress);
}

FindRent::~FindRent()
{
}

void FindRent::getData()
{
    QStringList vList;
    vList << mInformation.sqlWhere()
          << mArea.sqlWhere()
          << mType.sqlWhere()
          << mPrice.sqlWhere();
    QStringList vWheres;
    vWheres
            << (mAddress.data().isLocality() ? QString("microdistrict_fk = %1").arg(mAddress.data().locality().at(0)) : QString(""))
            << (mAddress.data().isStreet() ? QString("street_fk = %1").arg(mAddress.data().street().at(0)) : QString(""))
            << (mAddress.data().isNumber1() ? QString("number1 = '%1'").arg(mAddress.data().number1()) : QString(""))
            << (mAddress.data().isNumber2() ? QString("number2 = '%1'").arg(mAddress.data().number2()) : QString(""))
            << (mAddress.data().isRoom() ? QString("room = '%1'").arg(mAddress.data().room()) : QString(""));
    vWheres.removeAll("");
    if (vWheres.count())
    {
        vList << QString("(%1)").arg(vWheres.join(" AND "));
    }

    vList.removeAll("");
    vList.removeAll("()");
    setSql(vList.join(" AND "));
    vList.clear();

    vList << mInformation.joinWhere()
          << mArea.joinWhere()
          << mType.joinWhere()
          << mPrice.joinWhere();
    setJoin(vList.join(" "));
}
