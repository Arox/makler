#ifndef GENERALWINDOW_H
#define GENERALWINDOW_H

#include <QMainWindow>
#include <QList>

#include <QTimer>

#include "widgetforcontrol.h"
namespace Ui {
class GeneralWindow;
}

class GeneralWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GeneralWindow(QWidget *parent = 0);
    ~GeneralWindow();

private:
    Ui::GeneralWindow *ui;
};

#endif // GENERALWINDOW_H
