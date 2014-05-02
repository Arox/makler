#include "tablemodelapartment.h"

#include <QApplication>
#include <QColor>
#include <QPushButton>
#include "language.h"

TableModelApartment::TableModelApartment(QObject *parent):
    TableModel(parent)
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
             << TRANSLATE("Цена");       //9
}

QVariant TableModelApartment::data(const QModelIndex &index, int role) const
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
        return TableModel::data(index, role);
    }
}

void TableModelApartment::load()
{
    QStringList vSql;
    vSql << select()
         <<", roomstable.name as rooms,"
         << "typeapartment.level as level, typeapartment.maxlevel as levels,"
         << "static_dictionaries.name as fund, area.total as total,"
         << "area.floor as floor, area.kitchen as kitchen,"
         << "s1.option, price.price as price"
         << ",address.city_fk as city_fk, address.number1 as number1, address.number2 as number2"
         << ",address.street_fk as street_fk, address.microdistrict_fk as microdistrict_fk"
         << ",address.city as city, address.street as street, address.number as number"
         <<                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             from()
         << "INNER JOIN types ON objects.type_fk = types.id"
         << "LEFT JOIN typeapartment ON typeapartment.object_fk = objects.id"
         << "LEFT JOIN static_dictionaries ON static_dictionaries.id = typeapartment.fund_fk"
         << "LEFT JOIN static_dictionaries as roomstable ON roomstable.id = typeapartment.apartment_fk"
         << "LEFT JOIN area ON area.object_fk = objects.id"
         << "LEFT JOIN (SELECT dictionaries.name as option, objects.id as id, dictionaries.id as option_id FROM objects LEFT JOIN optionvalue ON objects.id = optionvalue.object_fk"
         << "INNER JOIN dictionaries ON optionvalue.value_fk = dictionaries.id AND dictionaries.parent = 24) as s1 ON s1.id = objects.id"

         //<< "LEFT JOIN optionvalue ON objects.id = optionvalue.object_fk"

         << "LEFT JOIN price ON price.object_fk = objects.id"

         <<  "LEFT JOIN"
         << "(SELECT address_city.id as city_fk, address.number1 as number1, address.number2 as number2"
         << ",address_street.id as street_fk, address_microdistrict.id as microdistrict_fk"
         << ",address_city.name as city, address_street.name as street"
         << ",(case "
         << "WHEN address.number2 like '' THEN address.number1"
         << "WHEN address.number2 is NULL THEN address.number1"
         << "ELSE address.number1 || '/' || address.number2 "
         << "END) as number"
         << ", object_fk"
         << "FROM address "
         << "LEFT JOIN address_street ON address_street.id = address.street_fk "
         << "LEFT JOIN address_microdistrict ON address_microdistrict.id = address.microdistrict_fk"
         << "LEFT JOIN address_city ON address_street.city_fk = address_city.id OR address_microdistrict.city_fk = address_city.id"
         << ") as address"
         << "ON address.object_fk = objects.id"

         << where("types.type = 'apartment'")
         << "ORDER BY objects.id desc";
     mData = execQuery(vSql.join(" "));
}

