#include "basestablemodel.h"
#include "language.h"

BasesTableModel::BasesTableModel(QObject *parent) :
    TableModel(parent)
{
    mHeaders << TRANSLATE("№")       //0
             << TRANSLATE("Хост")    //1
             << TRANSLATE("Имя");    //2
}

QVariant BasesTableModel::data(const QModelIndex &index, int role) const
{
    int vColumn = index.column();
    switch(role)
    {
    case Qt::DisplayRole:
        switch(vColumn)
        {
        case 0:
            return mData[index.row()]["id"];
        case 1:
            return mData[index.row()]["host"];
        case 2:
            return mData[index.row()]["name"];
        default:
            return QVariant();
        }
        break;
    default:
        return TableModel::data(index, role);
    }
}

void BasesTableModel::load()
{
    QStringList vSql;
    vSql << "SELECT * FROM bases";
     mData = execQuery(vSql.join(" "));
}
