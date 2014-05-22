#ifndef CONNECTTODATABASE_H
#define CONNECTTODATABASE_H

#include <QWidget>

#include "mylineedit.h"

namespace Ui {
class ConnectToDatabase;
}

class ConnectToDatabase : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectToDatabase(QWidget *parent = 0);
    ~ConnectToDatabase();

    /*Q_PROPERTY(QString host READ host WRITE setHost)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(int port READ port WRITE setPort)
    Q_PROPERTY(QString login READ login WRITE setLogin)
    Q_PROPERTY(QString password READ password WRITE setPassword)*/

    void setHost(QString aHost);
    QString host();

    void setName(QString aName);
    QString name();

    void setPort(int aPort);
    int port();

    void setLogin(QString aLogin);
    QString login();

    void setPassword(QString aPassword);
    QString password();

    bool isConnect();
public slots:
    void readSettings();
    void saveSettings();
    void showisConnect();
private:
    Ui::ConnectToDatabase *ui;
    MyLineEdit mHost;
    MyLineEdit mName;
    MyLineEdit mPort;
    MyLineEdit mPassword;
    MyLineEdit mLogin;
};

#endif // CONNECTTODATABASE_H
