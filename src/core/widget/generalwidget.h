#ifndef GENERALWIDGET_H
#define GENERALWIDGET_H

#include "widgetforcontrol.h"

class GeneralWidget : public WidgetForControl
{
    Q_OBJECT
public:
    explicit GeneralWidget(QWidget *parent = 0);
    virtual void load(int aId = -1) = 0;
signals:
    
public slots:
    
};

#endif // GENERALWIDGET_H
