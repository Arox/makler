#ifndef TABLEMODELRENT_H
#define TABLEMODELRENT_H


#include <QStringList>

#include "globalsbase.h"
#include "tablemodel.h"

class TableModelRent : public TableModel
{
    Q_OBJECT
public:
    TableModelRent(QObject *parent = 0);

    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual void load();
};

#endif // TABLEMODELRENT_H
