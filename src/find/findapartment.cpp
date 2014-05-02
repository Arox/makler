#include "findapartment.h"

#include <QStringList>
FindApartment::FindApartment(QWidget *parent) :
    FindWidget(parent)
{
    mInformation.setNameDictionary("apartment", 4, true);
    addWidget(&mInformation);
    addWidget(&mArea);
    addWidget(&mType);
    addWidget(&mPrice);
    addWidget(&mAddress);
}

FindApartment::~FindApartment()
{
}

void FindApartment::getData()
{
    QStringList vList;
    vList << mInformation.sqlWhere()
          << mArea.sqlWhere()
          << mType.sqlWhere()
          << mPrice.sqlWhere()
          << mAddress.sqlWhere()
             ;
    vList.removeAll("");
    vList.removeAll("()");
    setSql(vList.join(" AND "));
    vList.clear();

    vList << mInformation.joinWhere()
          << mArea.joinWhere()
          << mType.joinWhere()
          << mPrice.joinWhere()
          << mAddress.joinWhere();
    setJoin(vList.join(" "));
}
