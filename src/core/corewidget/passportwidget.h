#ifndef PASSPORTWIDGET_H
#define PASSPORTWIDGET_H

#include <QWidget>
#include "mainwidget.h"

namespace Ui {
class PassportWidget;
}

class PassportWidget : public MainWidget
{
    Q_OBJECT
    
public:
    explicit PassportWidget(QWidget *parent = 0);
    ~PassportWidget();
    void load(int aIdObjects);
public slots:
    void save();
private:
    Ui::PassportWidget *ui;
    int mId;
    int mIdObjects;
};

#endif // PASSPORTWIDGET_H
