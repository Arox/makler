#ifndef CLIENTHEADERALTERNATIVEWIDGET_H
#define CLIENTHEADERALTERNATIVEWIDGET_H

#include <QWidget>
#include <QSignalMapper>
#include <QLineEdit>
#include <QList>

#include "mainwidget.h"

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
protected slots:
    void saveClient(int aNumber);


private slots:
    void on_mpDiler_clicked();

private:
    Ui::ClientHeaderAlternativeWidget *ui;
    int mId;
    int mIdClients[5];
    int mIdObjects;
    QList<QLineEdit*> mNames;
    QList<QLineEdit*> mPhones;
    QSignalMapper mMapper;
};

#endif // CLIENTHEADERALTERNATIVEWIDGET_H
