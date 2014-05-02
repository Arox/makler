#include "findhome.h"

#include <QStringList>

FindHome::FindHome(QWidget *parent) :
    FindWidget(parent)
{
    mInformation.setNameDictionary("home", 4, true);
    addWidget(&mInformation);
    addWidget(&mArea);
    addWidget(&mType);
    addWidget(&mPrice);
    addWidget(&mHomeAddress);
    addWidget(&mAddress);
}

FindHome::~FindHome()
{
}

void FindHome::getData()
{
    QStringList vList;
    vList << mInformation.sqlWhere()
          << mArea.sqlWhere()
          << mType.sqlWhere()
          << mPrice.sqlWhere()
          << mHomeAddress.sqlWhere()
          << mAddress.sqlWhere();
    vList.removeAll("");
    vList.removeAll("()");
    setSql(vList.join(" AND "));
    vList.clear();

    vList << mInformation.joinWhere()
          << mArea.joinWhere()
          << mType.joinWhere()
          << mPrice.joinWhere()
          << mHomeAddress.joinWhere()
          << mAddress.joinWhere();
   setJoin(vList.join(" "));
}
