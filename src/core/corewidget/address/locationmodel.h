#ifndef LOCATIONMODEL_H
#define LOCATIONMODEL_H

#include "globalsbase.h"
#include "generalsettingslocation.h"

#include <QAbstractItemModel>
#include <QMap>

class LocationModel : public QAbstractItemModel
{
    Q_OBJECT
    ResponseType mData;
    int mType;
public:
    explicit LocationModel(QObject *parent = 0);
    virtual int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual QModelIndex	index(int row, int column, const QModelIndex & = QModelIndex()) const;
    virtual QModelIndex	parent(const QModelIndex &) const;
    virtual int	rowCount(const QModelIndex & parent = QModelIndex()) const;

    void setType(int aType);
    bool isType(int aType);
protected:
    virtual QString query() = 0;
    void load();
private:
    QString sqlQuery();
signals:

public slots:

};

#endif // LOCATIONMODEL_H
