#ifndef TABLEMODELAPARTMENTCLIENT_H
#define TABLEMODELAPARTMENTCLIENT_H

#include "findtablemodel.h"

class TableModelApartmentClient : public FindTableModel
{
    Q_OBJECT
public:
    TableModelApartmentClient(QObject* apPerent = 0);
    virtual void addToOrder();
    virtual void load();
    void clear();
    virtual QVariant data(const QModelIndex &index, int role) const;
};

#endif // TABLEMODELAPARTMENTCLIENT_H
