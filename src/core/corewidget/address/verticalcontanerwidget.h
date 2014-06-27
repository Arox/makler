#ifndef VERTICALCONTANERWIDGET_H
#define VERTICALCONTANERWIDGET_H

#include "locationselect.h"

#include <QWidget>
#include <QLabel>
namespace Ui {
class VerticalContanerWidget;
}

class VerticalContanerWidget : public QWidget
{
    Q_OBJECT
    LocationSelect* mpData;
public:
    explicit VerticalContanerWidget(QString vText, LocationSelect* apSelect, QWidget *parent = 0);
    ~VerticalContanerWidget();
    LocationSelect* data() const;
private:
    Ui::VerticalContanerWidget *ui;
};

#endif // VERTICALCONTANERWIDGET_H
