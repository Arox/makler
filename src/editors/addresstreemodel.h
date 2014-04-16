#ifndef ADDRESSTREEMODEL_H
#define ADDRESSTREEMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

class AddressTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit AddressTreeModel(QObject *parent = 0);
    /*virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);

    virtual int rowCount(const QModelIndex &parent=QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent) const;

    virtual QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;*/
signals:

public slots:

};

#endif // ADDRESSTREEMODEL_H
