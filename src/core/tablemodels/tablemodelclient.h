#ifndef TABLEMODELCLIENT_H
#define TABLEMODELCLIENT_H

#include <QStringList>

#include "globalsbase.h"
#include "tablemodel.h"

class TableModelClient : public TableModel
{
    Q_OBJECT
public:
    TableModelClient(QObject *parent = 0);

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual void load();
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    int typeObject(int aRow);
signals:
    void changeData(const QModelIndex&);
};

#endif // TABLEMODELCLIENT_H
