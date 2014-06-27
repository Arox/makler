#ifndef STREETWIDGET_H
#define STREETWIDGET_H

#include "locationselect.h"
#include "streetmodel.h"

class StreetWidget : public LocationSelect
{
    Q_OBJECT
    StreetModel* mpModel;
public:
    explicit StreetWidget(QWidget *parent = 0);
    void load(int aCityFk);
protected:
    virtual LocationModel* modelSelect();
    virtual LocationModel* modelCompleter();
signals:

public slots:

};

#endif // STREETWIDGET_H
