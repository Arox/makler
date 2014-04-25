#include "tablemodelclient.h"

#include <QApplication>
#include <QColor>
#include <QPushButton>

TableModelClient::TableModelClient(QObject *parent):
    TableModel(parent)
{
    mHeaders << TRANSLATE("№")          //0
             << TRANSLATE("Имя")    //1
             << TRANSLATE("Район")      //2
             << TRANSLATE("Объект")       //3
             << TRANSLATE("Цена")       //4
             ;                     //5
}

QVariant TableModelClient::data(const QModelIndex &index, int role) const
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
            return mData[index.row()]["fio"];
        case 2:
            return mData[index.row()]["locations"];
        case 3:
            return mData[index.row()]["type"];
        case 4:
            return QString("%1").arg(mData[index.row()]["price"].toInt());
        default:
            return QVariant();
        }
        break;
    default:
        return TableModel::data(index, role);
    }
}

bool TableModelClient::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column() == 2)
    {
        execQuery(QString("UPDATE orders SET locations = '%1' WHERE object_fk = %2")
                  .arg(value.toString())
                  .arg(id(index.row())));
        mData[index.row()].setData("locations", value);
        //return true;
    }
    if (index.column() == 3)
    {
        execQuery(QString("UPDATE orders SET type = '%1' WHERE object_fk = %2")
                  .arg(value.toString())
                  .arg(id(index.row())));
        mData[index.row()].setData("type", value);
        //return true;
    }
    if (index.column() == 4)
    {
        execQuery(QString("UPDATE orders SET price = '%1' WHERE object_fk = %2")
                  .arg(value.toString())
                  .arg(id(index.row())));
        mData[index.row()].setData("price", value);
        //emit changeData(index);
        //return true;
    }
    //return true;
    return TableModel::setData(index, value, role);
}

Qt::ItemFlags TableModelClient::flags(const QModelIndex &index) const
{
       if ((index.column() == 2 || index.column() == 4 || index.column() == 3))
       {
           return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
       }
       return TableModel::flags(index);
}

int TableModelClient::typeObject(int aRow)
{
    return mData[aRow]["type_fk"].toInt();
}

void TableModelClient::load()
{
    QStringList vSql;
    vSql //<< "SELECT DISTINCT ON (id, status, type, locations, price) objects.id as id, objects.status as status, objects.agent_fk as agent_fk"
         << select()
         << ",orders.type_fk as type_fk"
         << ",orders.locations as locations, orders.price as price"
         << ",min(clients.fio) as fio"
         << ",orders.type as type"
         << from()
         << "INNER JOIN types ON objects.type_fk = types.id"
         << "LEFT JOIN orders ON orders.object_fk = objects.id"
         << "LEFT JOIN clientheader ON clientheader.object_fk = objects.id"
         << "INNER JOIN clients ON clientheader.id = clients.header_fk"
         << where("types.type = 'client'")
         //<< "AND types.type = 'client'"
         << "group by objects.id, objects.status, orders.type_fk, orders.locations, orders.price, mans.sername, mans.name, mans.patronymic, orders.type"
         << "ORDER BY objects.id desc";

     mData = execQuery(vSql.join(" "));
}

