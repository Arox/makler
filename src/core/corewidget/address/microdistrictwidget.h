#ifndef MICRODISTRICTWIDGET_H
#define MICRODISTRICTWIDGET_H

#include "locationselect.h"
#include "microdistrictmodel.h"


class MicrodistrictWidget : public LocationSelect
{
    Q_OBJECT
    MicrodistrictModel* mpModel;
public:
    explicit MicrodistrictWidget(QWidget *parent = 0);
    void load(int aCityFk);
protected:
    virtual LocationModel* modelSelect();
    virtual LocationModel* modelCompleter();
signals:

public slots:

};

#endif // MICRODISTRICTWIDGET_H
