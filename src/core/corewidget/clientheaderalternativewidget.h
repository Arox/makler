#ifndef CLIENTHEADERALTERNATIVEWIDGET_H
#define CLIENTHEADERALTERNATIVEWIDGET_H

#include <QWidget>
#include <QSignalMapper>
#include <QLineEdit>
#include <QList>

#include "mainwidget.h"
#include "clientsphonewidget.h"

namespace Ui {
class ClientHeaderAlternativeWidget;
}

class ClientHeaderAlternativeWidget : public MainWidget
{
    Q_OBJECT
    
public:
    explicit ClientHeaderAlternativeWidget(QWidget *parent = 0);
    ~ClientHeaderAlternativeWidget();
    void load(int aAgent, int aIdObjects);
signals:
    void changed();
public slots:
    void save();
private slots:
    void on_mpDiler_clicked();

    void on_mpCancel_clicked();

    void on_mpSave_clicked();

private:
    Ui::ClientHeaderAlternativeWidget *ui;
    int mId;
    int mIdObjects;
    ClientsPhoneWidget* mpContact;
};

#endif // CLIENTHEADERALTERNATIVEWIDGET_H
