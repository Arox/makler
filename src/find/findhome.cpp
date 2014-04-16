#include "findhome.h"
#include "ui_findhome.h"

#include <QStringList>
#include "globalsbase.h"

FindHome::FindHome(QWidget *parent) :
    WidgetForControl(parent),
    ui(new Ui::FindHome)
{
    ui->setupUi(this);
    mInformation.setNameDictionary("home", 4, true);
    ui->mainLayout->insertWidget(0, &mInformation);
    ui->mainLayout->insertWidget(0, &mArea);
    ui->mainLayout->insertWidget(0, &mType);
    ui->mainLayout->insertWidget(0, &mPrice);
    ui->mainLayout->insertWidget(0, &mHomeAddress);
    ui->mainLayout->insertWidget(0, &mAddress);
}

FindHome::~FindHome()
{
    delete ui;
}

QString FindHome::sql()
{
    return mSql;
}

QString FindHome::join()
{
    return mJoin;
}

void FindHome::on_mpFind_clicked()
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

void FindHome::on_mpBack_clicked()
{
    emit back(this);
}

QString FindHome::name()
{
    return TRANSLATE("Поиск дома/участка");
}

QSize FindHome::minSize()
{
    return QSize(0, 0);
}

QSize FindHome::size()
{
    return QSize(880, 600);
}

QSize FindHome::maxSize()
{
    return QSize(0, 0);
}

QString FindHome::idName()
{
    return "findhome";
}
