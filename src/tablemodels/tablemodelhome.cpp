#include "tablemodelhome.h"
#include "globalsbase.h"

#include <QApplication>

TableModelHome::TableModelHome(QObject *parent):
    TableModel(parent)
{
    mHeaders << TRANSLATE("№")          //0
             //<< TRANSLATE("Район")    //1
             << TRANSLATE("Тип")       //3
             << TRANSLATE("Адрес")      //2

             << TRANSLATE("Соток")       //4
             << TRANSLATE("год постройки")      //5
             << TRANSLATE("Общая")      //6
             << TRANSLATE("Жилая")      //7
             << TRANSLATE("Кухня")  //8
             << TRANSLATE("Цена")       //9
             ;                     //10
}

QVariant TableModelHome::data(const QModelIndex &index, int role) const
{
    ResponseRecordType vRecord = mData[index.row()];
    int vColumn = index.column();
    switch(role)
    {
    case Qt::DisplayRole:
        if (isCoreColumn(index.column())) return TableModel::data(index, role);
        vColumn = realColumn(index.column());
        switch(vColumn)
        {
        case 0:
            return vRecord["id"];
        /*case 1:
            return QVariant();*/
        case 2:
            return QString("%1 %2 %3")
                    .arg(mData[index.row()]["city"].toString())
                    .arg(mData[index.row()]["street"].toString())
                    .arg(mData[index.row()]["number"].toString());
        case 1:
            return vRecord["isregion"].toBool() ? TRANSLATE("участок") : TRANSLATE("дом");
        case 3:
            return vRecord["area"];
        case 4:
            return vRecord["year"];
        case 5:
            return mData[index.row()]["total"];
        case 6:
            return mData[index.row()]["floor"];
        case 7:
            return mData[index.row()]["kitchen"];
        case 8:
            return QString("%1").arg(mData[index.row()]["price"].toInt());
        default:
            return QVariant();
        }
        break;
    default:
        return TableModel::data(index, role);
    }
    return QVariant();
}

void TableModelHome::load()
{
    QStringList vSql;
    vSql << select()
         << ", typehome.region as isregion,"
         << "typehome.area as area, typehome.year as year,"
         << "area.total as total,"
         << "area.floor as floor, area.kitchen as kitchen,"
         << "price.price as price"
         << ",address.city_fk as city_fk, address.number1 as number1, address.number2 as number2"
         << ",address.street_fk as street_fk, address.microdistrict_fk as microdistrict_fk"
         << ",address.city as city, address.street as street, address.number as number"
         << from()
         << "INNER JOIN types ON objects.type_fk = types.id"
         << "LEFT JOIN typeapartment ON typeapartment.object_fk = objects.id"
         << "LEFT JOIN area ON area.object_fk = objects.id"
         << "LEFT JOIN price ON price.object_fk = objects.id"
         << "LEFT JOIN typehome ON typehome.object_fk = objects.id"

         <<  "LEFT JOIN"
         << "(SELECT address_city.id as city_fk, address.number1 as number1, address.number2 as number2"
         << ",address_street.id as street_fk, address_microdistrict.id as microdistrict_fk"
         << ",address_city.name as city, address_street.name as street"
         << ",(case address.number2"
         << "WHEN '' THEN address.number1"
         << "WHEN NULL THEN address.number1"
         << "ELSE address.number1 || '/' || address.number2 "
         << "END) as number"
         << ", object_fk"
         << "FROM address "
         << "LEFT JOIN address_street ON address_street.id = address.street_fk "
         << "LEFT JOIN address_microdistrict ON address_microdistrict.id = address.microdistrict_fk"
         << "LEFT JOIN address_city ON address_street.city_fk = address_city.id OR address_microdistrict.city_fk = address_city.id"
         << ") as address"
         << "ON address.object_fk = objects.id"

         << where("types.type = 'home'")
         //<< "AND types.type = 'home'"
         << "ORDER BY objects.id desc";
     mData = execQuery(vSql.join(" "));
}
