#ifndef CLIENTSPHONEWIDGET_H
#define CLIENTSPHONEWIDGET_H

#include <QWidget>
#include <QVector>
#include <QScrollArea>
#include <QSignalMapper>

#include "mylineedit.h"

namespace Ui {
class ClientsPhoneWidget;
}

class ClientsPhoneWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientsPhoneWidget(QWidget *parent = 0);
    ~ClientsPhoneWidget();
    bool canSave();
public slots:
    void load(int aIdHeader);
    void save(int aIdHeader);
private:
    void saveClient(int aNumber, int aIdHeader);
private slots:
    void addContact();
    void removeContact(QWidget* apRemoveObject);
private:
    Ui::ClientsPhoneWidget *ui;
    QScrollArea* mpArea;

    QVector<MyLineEdit*> mpFios;
    QVector<MyLineEdit*> mpPhones;
    QVector<QLayout*> mContainers;
    QVector<int> mIds;
    QSignalMapper mDispatcher;
};

#endif // CLIENTSPHONEWIDGET_H
