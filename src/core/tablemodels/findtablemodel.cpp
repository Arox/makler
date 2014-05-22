#include "findtablemodel.h"

FindTableModel::FindTableModel(QObject *parent) :
    TableModel(parent)
  ,mIdObject(-1)
{
}


void FindTableModel::clear(QString aType)
{
  execQuery(QString("DELETE FROM objectstoorders WHERE object_in_order_fk = %1 AND object_fk IN (SELECT objects.id FROM objects INNER JOIN types ON types.type = '%2' AND objects.type_fk = types.id)")
            .arg(mIdObject)
            .arg(aType));

}

void FindTableModel::remove(int aId)
{
  execQuery(QString("DELETE FROM objectstoorders WHERE object_in_order_fk = %1 AND object_fk = %2")
            .arg(mIdObject)
            .arg(aId));
}

void FindTableModel::removeChecked()
{
  while (mChecked.count())
  {
      remove(mChecked.at(0));
      mChecked.removeAt(0);
  }
}

void FindTableModel::addToOrder()
{
}

void FindTableModel::setObjectId(int aObjectId)
{
  mIdObject = aObjectId;
  clearChecked();
}

QVariant FindTableModel::data(const QModelIndex &index, int role) const
{
    return TableModel::data(index, role);
}

void FindTableModel::load()
{
}

Qt::ItemFlags FindTableModel::flags(const QModelIndex &index) const
{
     return TableModel::flags(index);
}

bool FindTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  return TableModel::setData(index, value, role);
}

int FindTableModel::objectId()
{
    return mIdObject;
}

void FindTableModel::clearChecked()
{
    mChecked.clear();
}

void FindTableModel::addFilter(QString aWhere)
{
    TableModel::addFilter(aWhere);
    clearChecked();
}
