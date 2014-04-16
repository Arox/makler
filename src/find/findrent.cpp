#include "findrent.h"
#include "ui_findrent.h"

#include <QStringList>
#include "globalsbase.h"

FindRent::FindRent(QWidget *parent) :
    WidgetForControl(parent),
    ui(new Ui::FindRent)
{
    ui->setupUi(this);
    mInformation.setNameDictionary("rent", 4, true);
    ui->mainLayout->insertWidget(0, &mInformation);
    ui->mainLayout->insertWidget(0, &mArea);
    ui->mainLayout->insertWidget(0, &mType);
    ui->mainLayout->insertWidget(0, &mPrice);
    ui->mainLayout->insertWidget(0, &mHomeAddress);
    ui->mainLayout->insertWidget(0, &mAddress);
}

FindRent::~FindRent()
{
    delete ui;
}

QString FindRent::sql()
{
    return mSql;
}

QString FindRent::join()
{
    return mJoin;
}
void FindRent::on_mpFind_clicked()
{
    QStringList vList;
    vList << mInformation.sqlWhere()
          << mArea.sqlWhere()
          << mType.sqlWhere()
          << mPrice.sqlWhere()
          << mHomeAddress.sqlWhere()
          << mAddress.sqlWhere();
    vList.removeAll("");
    mSql = vList.join(" ");
    if (mSql.isEmpty()) mSql = " ";
    vList.clear();

    vList << mInformation.joinWhere()
          << mArea.joinWhere()
          << mType.joinWhere()
          << mPrice.joinWhere()
          << mHomeAddress.joinWhere()
          << mAddress.joinWhere();
    mJoin = vList.join(" ");
    emit back(this);
}

void FindRent::on_mpBack_clicked()
{
    emit back(this);
}

QString FindRent::name()
{
    return TRANSLATE("Поиск аренды");
}

QSize FindRent::minSize()
{
    return QSize(0, 0);
}

QSize FindRent::size()
{
    return QSize(880, 600);
}

QSize FindRent::maxSize()
{
    return QSize(0, 0);
}

QString FindRent::idName()
{
    return "findrent";
}
