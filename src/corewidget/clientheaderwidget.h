#ifndef CLIENTHEADERWIDGET_H
#define CLIENTHEADERWIDGET_H

#include <QWidget>
#include <QVector>

#include "mainwidget.h"
#include "clientsphonewidget.h"

namespace Ui {
class ClientHeaderWidget;
}

class ClientHeaderWidget : public MainWidget
{
    Q_OBJECT
    
public:
    explicit ClientHeaderWidget(QWidget *parent = 0);
    ~ClientHeaderWidget();

    void load(int aAgent, int aIdObjects);
    bool canSave();
signals:
    void changed();
public slots:
    void save();
private:
    Ui::ClientHeaderWidget *ui;
    ClientsPhoneWidget* mpContacts;
    int mId;
    int mIdObjects;
};

#endif // CLIENTHEADERWIDGET_H
