#ifndef TABLEMODELHOME_H
#define TABLEMODELHOME_H


#include <QStringList>

#include "globalsbase.h"
#include "tablemodel.h"

class TableModelHome : public TableModel
{
    Q_OBJECT
public:
    TableModelHome(QObject *parent = 0);

    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual void load();
};

#endif // TABLEMODELHOME_H
