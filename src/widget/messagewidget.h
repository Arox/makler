#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>
#include <QTimer>

#include "widgetforcontrol.h"
#include "../messages/viewpostswidget.h"

namespace Ui {
class MessageWidget;
}

class MessageWidget : public WidgetForControl
{
    Q_OBJECT
    
public:
    explicit MessageWidget(int aUser_fk, QWidget *parent = 0);
    ~MessageWidget();
    virtual QString name();
    virtual QSize minSize();
    virtual QSize maxSize();
    virtual QSize size();
    virtual QString idName();
private slots:
    void countMail();
private:
    Ui::MessageWidget *ui;
    int mUser_fk;
    ViewPostsWidget mViewPost;
    QTimer mTimer;
};

#endif // MESSAGEWIDGET_H
