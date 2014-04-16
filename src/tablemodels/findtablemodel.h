#ifndef FINDTABLEMODEL_H
#define FINDTABLEMODEL_H

#include <QList>
#include "tablemodel.h"

class FindTableModel : public TableModel
{
    Q_OBJECT
    int mIdObject;

public:
    explicit FindTableModel(QObject *parent = 0);
    virtual void addToOrder();
    virtual void load();

    void setObjectId(int aObjectId);
    void remove(int aId);
    virtual QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void removeChecked();
    int objectId();
    virtual void addFilter(QString aWhere);
protected:
    void clear(QString aType);
    void clearChecked();
public slots:
    
};

#endif // FINDTABLEMODEL_H
