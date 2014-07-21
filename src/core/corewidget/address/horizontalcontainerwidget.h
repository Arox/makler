#ifndef HORIZONTALCONTAINERWIDGET_H
#define HORIZONTALCONTAINERWIDGET_H

#include "locationselect.h"

#include <QWidget>
#include <QLabel>

namespace Ui {
class HorizontalContainerWidget;
}

class HorizontalContainerWidget : public QWidget
{
    LocationSelect* mpData;
public:
    LocationSelect* data() const;
    explicit HorizontalContainerWidget(QString vText, LocationSelect* apSelect, QWidget *parent = 0);
    ~HorizontalContainerWidget();

private:
    Ui::HorizontalContainerWidget *ui;
};

#endif // HORIZONTALCONTAINERWIDGET_H
