#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QStringList>

#include "globalsbase.h"

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

    bool mActive;
    bool mAside;
    bool mArchive;
    QString mWhereFilter;
    bool mIsButtons;
    int mCheckedsColumn;

    int mColumnShowAgent;
protected:
    QStringList mHeaders;
    ResponseType mData;
    QList<int> mChecked;

    QString select();
    QString from();
    QString where(QString aWhere);

    bool isCoreColumn(int aColumn) const;

    int realColumn(int aColumn) const;
    int coreColumn() const;
public:
    TableModel(QObject *parent = 0);

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);

    virtual int rowCount(const QModelIndex &parent=QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent) const;

    virtual QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    virtual void load();

    virtual bool active();
    virtual bool aside();
    virtual bool archive();

    virtual void setActive(bool aActive);
    virtual void setAside(bool aAside);
    virtual void setArchive(bool aArchive);
    virtual int id(int aRow) const;
    virtual QString status(int aRow);

    virtual void addFilter(QString aWhere);
    virtual void clearFilter();
    QString filter();

    void setButtonStatus(bool aStatus);

    QList<int> checkedId();
    bool checkedId(int aId) const;
    bool isChecked();
    void setColumnChecable(int aColumn);
    void setColumnAgent(int aColumn);
    void clearSelection();
    virtual void remove(int aId);
public slots:
    void changeActive();
    void changeAside();
    void changeArchive();
signals:
    void checkStateChange();
};

#endif // TABLEMODEL_H
