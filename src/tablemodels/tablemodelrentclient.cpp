#include "tablemodelrentclient.h"
#include <QApplication>
TableModelRentClient::TableModelRentClient(QObject *parent) :
    FindTableModel(parent)
{
    mHeaders << TRANSLATE("№")          //0
             << TRANSLATE("Комнаты")    //1
             << TRANSLATE("Адрес")      //2
             << TRANSLATE("Этаж")       //3
             << TRANSLATE("Фонд")       //4
             << TRANSLATE("Общая")      //5
             << TRANSLATE("Жилая")      //6
             << TRANSLATE("Кухня")      //7
             << TRANSLATE("Состояние")  //8
             << TRANSLATE("Цена")       //9
             ;                     //10
}

void TableModelRentClient::clear()
{
    FindTableModel::clear("rent");
}

void TableModelRentClient::addToOrder()
{
    clear();

    QStringList vSql;
    vSql //<< select()
         << QString("SELECT objects.id as id, %1").arg(objectId())
         << from()
         << QString("INNER JOIN objectstoorders ON objectstoorders.object_in_order_fk = %1 AND objects.id = objectstoorders.object_fk").arg(objectId())
         << "INNER JOIN types ON objects.type_fk = types.id"
         << "LEFT JOIN typeapartment ON typeapartment.object_fk = objects.id"
         << "LEFT JOIN area ON area.object_fk = objects.id"
         << "LEFT JOIN (SELECT dictionaries.name as option, objects.id as id FROM objects LEFT JOIN optionvalue ON objects.id = optionvalue.object_fk"
         << "INNER JOIN dictionaries ON optionvalue.value_fk = dictionaries.id AND dictionaries.parent = 24) as s1 ON s1.id = objects.id"
         << "LEFT JOIN price ON price.object_fk = objects.id"
         << "LEFT JOIN static_dictionaries as roomstable ON roomstable.id = typeapartment.apartment_fk"
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


         << where("types.type = 'rent'")
         //<< "AND types.type = 'rent'"
            ;
    execQuery(QString("INSERT INTO objectstoorders (object_fk, object_in_order_fk) (%1)")
              .arg(vSql.join(" ")));
}

QVariant TableModelRentClient::data(const QModelIndex &index, int role) const
{
    int vColumn = index.column();
    switch(role)
    {
    case Qt::DisplayRole:
        if (isCoreColumn(index.column())) return TableModel::data(index, role);
        vColumn = realColumn(index.column());
        switch(vColumn)
        {
        case 0:
            return mData[index.row()]["id"];
        case 1:
            return mData[index.row()]["rooms"];
        case 2:
            return QString("%1 %2 %3")
                    .arg(mData[index.row()]["city"].toString())
                    .arg(mData[index.row()]["street"].toString())
                    .arg(mData[index.row()]["number"].toString());
        case 3:
            return QString("%1/%2").arg(mData[index.row()]["level"].toInt()).arg(mData[index.row()]["levels"].toInt());
        case 4:
            return mData[index.row()]["fund"];
        case 5:
            return mData[index.row()]["total"];
        case 6:
            return mData[index.row()]["floor"];
        case 7:
            return mData[index.row()]["kitchen"];
        case 8:
            return mData[index.row()]["option"];
        case 9:
            return QString("%1").arg(mData[index.row()]["price"].toInt());
        default:
            return QVariant();
        }
        break;
    default:
        return FindTableModel::data(index, role);
    }
}

void TableModelRentClient::load()
{
    QStringList vSql;
    vSql << select()
         <<", roomstable.name as rooms,"
         << "typehome.region as isregion,"
         << "typehome.area as area, typehome.year as year,"
         << "area.total as total,"
         << "area.floor as floor, area.kitchen as kitchen,"
         << "s1.option, price.price as price"
         << ",address.city_fk as city_fk, address.number1 as number1, address.number2 as number2"
         << ",address.street_fk as street_fk, address.microdistrict_fk as microdistrict_fk"
         << ",address.city as city, address.street as street, address.number as number"
         << from()
         //<< QString("INNER JOIN objectstoorders ON objectstoorders.object_in_order_fk = %1 AND objects.id = objectstoorders.object_fk").arg(objectId())
         << "INNER JOIN types ON objects.type_fk = types.id"
         << "LEFT JOIN typeapartment ON typeapartment.object_fk = objects.id"
         << "LEFT JOIN area ON area.object_fk = objects.id"
         << "LEFT JOIN static_dictionaries as roomstable ON roomstable.id = typeapartment.apartment_fk"
         << "LEFT JOIN (SELECT dictionaries.name as option, objects.id as id FROM objects LEFT JOIN optionvalue ON objects.id = optionvalue.object_fk"
         << "INNER JOIN dictionaries ON optionvalue.value_fk = dictionaries.id AND dictionaries.parent = 24) as s1 ON s1.id = objects.id"
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


         << where("types.type = 'rent'")
         //<< "AND types.type = 'rent'"
         << "ORDER BY objects.id desc";
     mData = execQuery(vSql.join(" "));
}
