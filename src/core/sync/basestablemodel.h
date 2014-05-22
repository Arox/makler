#ifndef BASESTABLEMODEL_H
#define BASESTABLEMODEL_H

#include "tablemodel.h"

class BasesTableModel : public TableModel
{
    Q_OBJECT
public:
    explicit BasesTableModel(QObject *parent = 0);
    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual void load();
signals:

public slots:

};

#endif // BASESTABLEMODEL_H
