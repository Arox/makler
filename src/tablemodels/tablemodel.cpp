#include "tablemodel.h"

#include <QApplication>
#include <QColor>

TableModel::TableModel(QObject *parent):
    QAbstractTableModel(parent)
  , mIsButtons(true)
  , mCheckedsColumn(-1)
  , mColumnShowAgent(-1)
{
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
    case Qt::BackgroundColorRole:
        if (mData[index.row()]["status"].toString() == "A")
        {
            return QColor(255, 247, 14);
        }
        if (mData[index.row()]["status"].toString() == "S")
        {
            return QColor(174, 174, 174);
        }
        return QVariant();
    case Qt::CheckStateRole:
        if (index.column() == mCheckedsColumn)
        {
            if (checkedId(id(index.row())))
            {
                return Qt::Checked;
            }
            else
            {
                return Qt::Unchecked;
            }
        }
        return QVariant();
    case Qt::DisplayRole:
        if (index.column() == mColumnShowAgent)
        {
            return QString("%1 %2 %3")
                    .arg(mData[index.row()]["sername"].toString())
                    .arg(mData[index.row()]["name"].toString())
                    .arg(mData[index.row()]["patronymic"].toString());
        }
    default:
        return QVariant();
    }
    return QVariant();
}
bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column() == mCheckedsColumn)
    {
        if (role == Qt::CheckStateRole)
        {
            if (value.toBool())
            {
                mChecked.append(id(index.row()));
            }
            else
            {
                mChecked.removeAll(id(index.row()));
            }
            emit checkStateChange();
            return true;
        }
    }
    return false;
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mData.count();
}
int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return  mHeaders.count() + coreColumn();
}

QVariant TableModel::headerData(int section,Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
               return QVariant();
       if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
       {
           if (mColumnShowAgent == section)
           {
               return TRANSLATE("Агент");
           }
           if (mIsButtons && (section == (columnCount(QModelIndex()) - 1)))
           {
               return "";
           }
           return mHeaders.at(realColumn(section)); // заголовки столбцов
       }
       else
       {
           return QString("%1").arg( section + 1 ); // возвращаем номера строк
       }
}
Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
        if (index.column() == mCheckedsColumn)
        {
            return QAbstractItemModel::flags(index) | Qt::ItemIsUserCheckable;
        }
       return QAbstractItemModel::flags(index);// | Qt::ItemIsEditable;
}

void TableModel::load()
{
}

bool TableModel::active()
{
    return mActive;
}

bool TableModel::aside()
{
    return mAside;
}

bool TableModel::archive()
{
    return mArchive;
}

void TableModel::setActive(bool aActive)
{
    mActive = aActive;
}

void TableModel::setAside(bool aAside)
{
    mAside = aAside;
}

void TableModel::setArchive(bool aArchive)
{
    mArchive = aArchive;
}

void TableModel::changeActive()
{
    setActive(!active());
    //load();
}

void TableModel::changeAside()
{
    setAside(!aside());
    //load();
}

void TableModel::changeArchive()
{
    setArchive(!archive());
    //load();
}

int TableModel::id(int aRow) const
{
    return mData[aRow]["id"].toInt();
}

QString TableModel::status(int aRow)
{
    if (mData.count()) return mData[aRow]["status"].toString();
    else return QString();
}

void TableModel::addFilter(QString aWhere)
{
    mWhereFilter = aWhere;
}

QString TableModel::filter()
{
    return mWhereFilter;
}

void TableModel::setButtonStatus(bool aStatus)
{
    mIsButtons = aStatus;
}

QList<int> TableModel::checkedId()
{
  return mChecked;
}

bool TableModel::isChecked()
{
  return mChecked.count();
}

bool TableModel::checkedId(int aId) const
{
    return mChecked.contains(aId);
}

void TableModel::setColumnChecable(int aColumn)
{
    mCheckedsColumn = aColumn;
}

QString TableModel::select()
{
    return "SELECT objects.id as id, objects.status as status, mans.sername as sername, mans.\"name\" as \"name\", mans.patronymic as patronymic, objects.agent_fk as agent_fk";
}

QString TableModel::from()
{
    return "FROM objects LEFT JOIN mans ON mans.id = objects.agent_fk";
}

QString TableModel::where(QString aWhere)
{
    QStringList vWheres;
    vWheres
            << (active() ? QString("objects.status is NULL") : QString(""))
            << (aside() ? QString("objects.status = 'A'") : QString(""))
            << (archive() ? QString("objects.status = 'S'") : QString(""));

    vWheres.removeAll("");

    if (!vWheres.count())
    {
        vWheres << QString("(FALSE)");
    }

    QStringList vSql;
    vSql //<< "WHERE"
         << (vWheres.count() ? QString("(%1)").arg(vWheres.join(" OR ")) : QString(""))
         << (filter().isEmpty() ? "" : QString("%1").arg(filter()))
         << aWhere;
    vSql.removeAll("");
    return QString("WHERE %1").arg(vSql.join(" AND "));
}

void TableModel::setColumnAgent(int aColumn)
{
    mColumnShowAgent = aColumn;
}

bool TableModel::isCoreColumn(int aColumn) const
{
    if (aColumn == mColumnShowAgent)
    {
        return true;
    }
    return false;
}

int TableModel::realColumn(int aColumn) const
{
    int vOutColumn = 0;
    if ((mColumnShowAgent >= 0) && (aColumn >= mColumnShowAgent)) vOutColumn += 1;
    return aColumn - vOutColumn;
}

int TableModel::coreColumn() const
{
    int vCount = 0;
    if (mIsButtons) vCount+=1;
    if (mColumnShowAgent >= 0) vCount += 1;
    return vCount;
}

void TableModel::remove(int aId)
{
    execQuery(QString("DELETE FROM objects WHERE id = %1").arg(aId));
}

void TableModel::clearSelection()
{
    mChecked.clear();
}
