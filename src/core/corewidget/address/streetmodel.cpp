#include "streetmodel.h"

StreetModel::StreetModel(int aCityFk, QObject *parent) :
    LocationModel(parent)
{
    mSql = QString("select id, name from address_street where city_fk = %1")
            .arg(aCityFk);
    load();
}

QString StreetModel::query()
{
    return mSql;
}
