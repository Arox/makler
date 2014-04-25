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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*QString vPath = QString("%1/%2")
            .arg(a.applicationDirPath())
            .arg("styles/mainstyle.qss");

    QFile vFile(vPath);
    if (vFile.open(QIODevice::ReadOnly))
    {
        QString vStyle = QString(vFile.readAll());
        a.setStyleSheet(vStyle);
    }*/

    /*QDateTime vDate = QDateTime::currentDateTime();
    if (vDate.date() > QDate(2014, 5, 31)) return -1;*/

    //a.setLibraryPaths(QStringList("C:\\projects\\build-realtor-Desktop_Qt_5_1_1_MinGW_32bit-Release\\release"));
    //QMessageBox::information(0, "", a.libraryPaths().join(' '));

    Styles::inizialize();
    if (Styles::styles().count())
    {
        Styles::setStyle(Styles::styles().at(0));
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", Settings::databaseLocalName());
    db.setDatabaseName("base.sqlite");

    /*QString vType = Settings::databaseType();
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
         db.setPort(vPort);*/
         if (!db.open())
         {
             QMessageBox::information(0, QString(""), db.lastError().text());
         }
    QTime vTime = QTime::currentTime();

    MainWindow w;
    qDebug() << vTime;
    w.load(new TopMenu(w.user_fk(), w.roles()));
    w.show();
    qDebug() << QTime::currentTime();
    qDebug() << QTime::currentTime().addSecs(-1*(vTime.hour() * 3600 + vTime.minute()* 60 + vTime.second()));
    return a.exec();
}
