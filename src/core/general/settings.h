#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QString>

class Settings : public QObject
{
#define APPNAME "Rieltor"
    Q_OBJECT
    static QSettings mBaseSettings;
public:
    /*explicit Settings(QObject *parent = 0);
    ~Settings();*/
    static  void createDefaultValue();

    static QString databaseName() ;
    static QString databaseHost() ;
    static QString databaseLogin() ;
    static QString databasePassword() ;
    static int databasePort() ;
    static QString databaseLocalName() ;
    static QString databaseType() ;
    static QString site() ;

    static void setDatabaseName(QString) ;
    static void setDatabaseHost(QString) ;
    static void setDatabaseLogin(QString) ;
    static void setDatabasePassword(QString) ;
    static void setDatabasePort(int) ;
    static void setDatabaseType(QString) ;
    static void setSite(QString aSite) ;
signals:
    
public slots:
    
};

#endif // SETTINGS_H