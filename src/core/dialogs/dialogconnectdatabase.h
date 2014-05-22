#ifndef DIALOGCONNECTDATABASE_H
#define DIALOGCONNECTDATABASE_H

#include <QDialog>
#include "connecttodatabase.h"

namespace Ui {
class DialogConnectDatabase;
}

class DialogConnectDatabase : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConnectDatabase(QWidget *parent = 0);
    ~DialogConnectDatabase();

private slots:
    void on_mpOk_clicked();

    void on_mpExit_clicked();

private:
    Ui::DialogConnectDatabase *ui;
    ConnectToDatabase mConnectWidget;
};

#endif // DIALOGCONNECTDATABASE_H
