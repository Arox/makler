#ifndef BUTTONTOPMENUWIDGET_H
#define BUTTONTOPMENUWIDGET_H

#include <QWidget>

namespace Ui {
class ButtonTopMenuWidget;
}

class ButtonTopMenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ButtonTopMenuWidget(QString aText, QWidget *parent = 0);
    ~ButtonTopMenuWidget();
signals:
    void activated();
public slots:
    void setActive(bool aFlag);
private:
    Ui::ButtonTopMenuWidget *ui;
};

#endif // BUTTONTOPMENUWIDGET_H
