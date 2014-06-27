#include "microdistrictmodel.h"

MicrodistrictModel::MicrodistrictModel(int aCityFk, QObject *parent) :
    LocationModel(parent)
{
    mSql = QString("select id, name from address_microdistrict where city_fk = %1")
            .arg(aCityFk);
    load();
}

QString MicrodistrictModel::query()
{
    return mSql;
}
