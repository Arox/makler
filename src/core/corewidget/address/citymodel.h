#ifndef CITYMODEL_H
#define CITYMODEL_H

#include "locationmodel.h"

class CityModel : public LocationModel
{
    Q_OBJECT
    QString mSql;
public:
    enum TypeCity
    {
        City,
        Garden
    };

    explicit CityModel(TypeCity aType, QObject *parent = 0);
protected:
    virtual QString query();
signals:

public slots:

};

#endif // CITYMODEL_H
