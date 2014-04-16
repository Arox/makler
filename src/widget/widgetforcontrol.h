#ifndef WIDGETFORCONTROL_H
#define WIDGETFORCONTROL_H

#include <QWidget>
#include <QIcon>
#include <QPair>

class WidgetForControl : public QWidget
{
    Q_OBJECT

public:
    typedef QPair<WidgetForControl *, WidgetForControl *> SignalWidgetType;
    WidgetForControl(QWidget* apWidget = 0);

    virtual QString name() = 0;
    virtual QSize minSize() = 0;
    virtual QSize maxSize() = 0;
    virtual QSize size() = 0;
    virtual QString idName() = 0;
signals:
    void changeWidget(SignalWidgetType);
    void back(WidgetForControl*);
    //void close();
public slots:
    virtual void reload(WidgetForControl*);
};

#endif // WIDGETFORCONTROL_H
