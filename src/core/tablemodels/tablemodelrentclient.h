#ifndef TABLEMODELRENTCLIENT_H
#define TABLEMODELRENTCLIENT_H

#include "findtablemodel.h"

class TableModelRentClient : public FindTableModel
{
    Q_OBJECT
public:
    explicit TableModelRentClient(QObject *parent = 0);
    virtual void addToOrder();
    virtual void load();
    void clear();
    virtual QVariant data(const QModelIndex &index, int role) const;
public slots:
    
};

#endif // TABLEMODELRENTCLIENT_H
