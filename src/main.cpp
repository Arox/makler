#include "mainwindow.h"
#include <QApplication>

#include <QSqlDatabase>
#include <QStyleFactory>
#include "classicmenuwidget.h"
#include <QFile>
#include "settings.h"
#include "general/globalsbase.h"
#include <QDebug>
#include <QSqlError>
#include <QDateTime>
#include <QMessageBox>
#include <QStringList>
#include "topmenu.h"
#include "styles.h"
#include <QDebug>
#include <QSettings>
#include "dialogconnectdatabase.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qApp->setApplicationVersion("1.0.1");
    Styles::inizialize();
    if (Styles::styles().count())
    {
        Styles::setStyle(Styles::styles().at(0));
    }
    bool vConnect = false;
    while (!vConnect)
    {
        QString vType = Settings::databaseType();
        QString vHost = Settings::databaseHost();
        QString vName = Settings::databaseName();
        QString vLogin = Settings::databaseLogin();
        QString vPassword = Settings::databasePassword();
        int vPort = Settings::databasePort();
        QSqlDatabase db = QSqlDatabase::addDatabase(vType, Settings::databaseLocalName());
             db.setHostName(vHost);
             db.setDatabaseName(vName);
             db.setUserName(vLogin);
             db.setPassword(vPassword);
             db.setPort(vPort);
        if (!db.open())
        {
            DialogConnectDatabase vDialogConnect;
            if (vDialogConnect.exec() != QDialog::Accepted)
            {
                return -1;
            }
        }
        else
        {
            vConnect = true;
        }
    }
    QTime vTime = QTime::currentTime();

    MainWindow w;
    qDebug() << vTime;
    w.load(WidgetForControl::SignalWidgetType(0, new TopMenu(w.user_fk(), w.roles())));
    w.show();
    qDebug() << QTime::currentTime();
    qDebug() << QTime::currentTime().addSecs(-1*(vTime.hour() * 3600 + vTime.minute()* 60 + vTime.second()));
    return a.exec();
}
