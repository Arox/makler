#ifndef TABLEMODELAPARTMENT_H
#define TABLEMODELAPARTMENT_H


#include <QStringList>

#include "globalsbase.h"
#include "tablemodel.h"

class TableModelApartment : public TableModel
{
    Q_OBJECT
public:
    TableModelApartment(QObject *parent = 0);

    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual void load();
};

#endif // TABLEMODELAPARTMENT_H
