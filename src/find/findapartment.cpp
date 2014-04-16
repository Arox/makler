#include "findapartment.h"
#include "ui_findapartment.h"

#include <QStringList>
#include "globalsbase.h"
FindApartment::FindApartment(QWidget *parent) :
    WidgetForControl(parent),
    ui(new Ui::FindApartment)
{
    ui->setupUi(this);
    mInformation.setNameDictionary("apartment", 4, true);
    ui->mainLayout->insertWidget(0, &mInformation);
    ui->mainLayout->insertWidget(0, &mArea);
    ui->mainLayout->insertWidget(0, &mType);
    ui->mainLayout->insertWidget(0, &mPrice);
    ui->mainLayout->insertWidget(0, &mAddress);


}

FindApartment::~FindApartment()
{
    delete ui;
}

void FindApartment::on_mpFind_clicked()
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
    mSql = vList.join(" AND ");
    vList.clear();
    if (mSql.isEmpty()) mSql = " ";

    vList << mInformation.joinWhere()
          << mArea.joinWhere()
          << mType.joinWhere()
          << mPrice.joinWhere()
          << mAddress.joinWhere();
    mJoin = vList.join(" ");
    emit back(this);
}

void FindApartment::on_mpBack_clicked()
{
    emit back(this);
}

QString FindApartment::sql()
{
    return mSql;
}

QString FindApartment::join()
{
    return mJoin;
}

QString FindApartment::name()
{
    return TRANSLATE("Поиск квартиры");
}

QSize FindApartment::minSize()
{
    return QSize(0, 0);
}

QSize FindApartment::size()
{
    return QSize(880, 600);
}

QSize FindApartment::maxSize()
{
    return QSize(0, 0);
}

QString FindApartment::idName()
{
    return "findapartment";
}
