#ifndef MICRODISTRICTMODEL_H
#define MICRODISTRICTMODEL_H

#include "locationmodel.h"

class MicrodistrictModel : public LocationModel
{
    Q_OBJECT
    QString mSql;
public:
    explicit MicrodistrictModel(int aCityFk, QObject *parent = 0);
protected:
    virtual QString query();
signals:

public slots:

};
#endif // MICRODISTRICTMODEL_H
