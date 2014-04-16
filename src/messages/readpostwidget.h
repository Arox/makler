#ifndef READPOSTWIDGET_H
#define READPOSTWIDGET_H

#include <QWidget>
#include "viewpostwidget.h"
#include "mainwidget.h"
namespace Ui {
class ReadPostWidget;
}

class ReadPostWidget : public MainWidget
{
    Q_OBJECT
    
public:
    explicit ReadPostWidget(int aIdUser, int aIdMessage, QWidget *parent = 0);
    ~ReadPostWidget();
private slots:
    void saveFile(QByteArray aArray);
    void on_mpClose_clicked();

private:
    Ui::ReadPostWidget *ui;
    ViewPostWidget mView;
};

#endif // READPOSTWIDGET_H
