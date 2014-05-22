#ifndef DIALOGFORWIDGETFORCONTROL_H
#define DIALOGFORWIDGETFORCONTROL_H

#include <QDialog>

namespace Ui {
class DialogForWidgetForControl;
}

class DialogForWidgetForControl : public QDialog
{
    Q_OBJECT

public:
    explicit DialogForWidgetForControl(QWidget *apChildren, QString aName, QWidget *parent = 0);
    ~DialogForWidgetForControl();

private:
    Ui::DialogForWidgetForControl *ui;
};

#endif // DIALOGFORWIDGETFORCONTROL_H
