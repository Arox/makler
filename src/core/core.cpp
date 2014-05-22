#include "core.h"
#include "mainwindow.h"

#include <QSqlDatabase>
#include "settings.h"
#include "general/globalsbase.h"
#include <QStringList>
#include "topmenu.h"
#include "styles.h"
#include "dialogconnectdatabase.h"

QMainWindow* getWindow()
{
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
                return NULL;
            }
        }
        else
        {
            vConnect = true;
        }
    }
    MainWindow* w = new MainWindow();
    w->load(WidgetForControl::SignalWidgetType(0, new TopMenu(w->user_fk(), w->roles())));

    return w;
}
