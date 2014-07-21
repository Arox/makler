#include "locationmodel.h"

using namespace location;

LocationModel::LocationModel(QObject *parent) :
    QAbstractItemModel(parent)
  ,mType(location::NORMAL)
{

}

void LocationModel::load()
{
    mData = execQuery(sqlQuery());
    if (mIcons.count()) mIcons.clear();
    mIcons.fill(QIcon(), mData.count());
}

int	LocationModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant LocationModel::data(const QModelIndex & index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if (index.column() == 0)
        {
            if (mData.count() > index.row())
            {
                return QString("  %1  ").arg(mData[index.row()]["name"].toString());
            }
        }
    }
    if (role == Qt::UserRole)
    {
        if (index.column() == 0)
        {
            if (mData.count() > index.row())
            {
                return mData[index.row()]["id"];
            }
        }
    }
    if (role == Qt::DecorationRole)
    {
        if (mIcons.count() > index.row() && index.row() >= 0)
        {
            return mIcons.at(index.row());
        }
    }
    return QVariant();
}

bool LocationModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::DecorationRole)
    {
        if (mIcons.count() > index.row())
        {
            mIcons[index.row()] = value.value<QIcon>();
        }
    }
}

QModelIndex	LocationModel::index(int row, int column, const QModelIndex &) const
{
    return createIndex(row, column);
}

QModelIndex	LocationModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int	LocationModel::rowCount(const QModelIndex &) const
{
    return mData.count();
}

QString LocationModel::sqlQuery()
{
    if (isType(FIND) && !isType(MULTISELECT))
    {
        return QString("SELECT -1 as id, '' as name UNION %1").arg(query());
    }
    else
    {
        return query();
    }
}

void LocationModel::setType(int aType)
{
    mType = aType;
}

bool LocationModel::isType(int aType)
{
    return mType & aType;
}
