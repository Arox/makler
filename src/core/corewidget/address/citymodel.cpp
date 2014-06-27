#include "citymodel.h"

CityModel::CityModel(TypeCity aType, QObject *parent) :
    LocationModel(parent)
{
    if (aType == City)
    {
        mSql = QString("select id as id, name as name from address_city where type_fk IN (select id from typecity where name='city')");
    }
    else
    {
        mSql = QString("select id as id, name as name from address_city where type_fk IN (select id from typecity where name='garden')");
    }
    load();
}

QString CityModel::query()
{
    return mSql;
}
