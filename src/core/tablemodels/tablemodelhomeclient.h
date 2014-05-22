#ifndef TABLEMODELHOMECLIENT_H
#define TABLEMODELHOMECLIENT_H

#include "findtablemodel.h"

class TableModelHomeClient : public FindTableModel
{
    Q_OBJECT
public:
    explicit TableModelHomeClient(QObject *parent = 0);
    virtual void addToOrder();
    virtual void load();
    void clear();
    virtual QVariant data(const QModelIndex &index, int role) const;
    
public slots:
    
};

#endif // TABLEMODELHOMECLIENT_H
