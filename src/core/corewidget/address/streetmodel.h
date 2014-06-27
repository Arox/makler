#ifndef STREETMODEL_H
#define STREETMODEL_H

#include "locationmodel.h"

class StreetModel : public LocationModel
{
    Q_OBJECT
    QString mSql;
public:
    explicit StreetModel(int aCityFk, QObject *parent = 0);
protected:
    virtual QString query();
signals:

public slots:

};

#endif // STREETMODEL_H
