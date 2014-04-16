#include "settings.h"

#ifdef WINDOWS_OS
    QSettings Settings::mBaseSettings(QString("HKEY_CURRENT_USER\\Software\\%1").arg(APPNAME), QSettings::NativeFormat);
#else
    QSettings Settings::mBaseSettings(QString("./%1.ini").arg(APPNAME), QSettings::IniFormat);
#endif

/*Settings::Settings(QObject *parent) :
    QObject(parent)
{
#ifdef WINDOWS_OS
    mpBaseSettings = new QSettings(QString("HKEY_CURRENT_USER\\Software\\%1").arg(APPNAME), QSettings::NativeFormat);
#else
    mpBaseSettings = new QSettings(QString("%APPDATA%\%1.ini").arg(APPNAME), QSettings::IniFormat);
#endif
    Q_ASSERT_X(mpBaseSettings, "don`t create setting class");
}*/

void Settings::createDefaultValue()
{
    mBaseSettings.setValue("database/name", "");
    mBaseSettings.setValue("database/host", "");
    mBaseSettings.setValue("database/port", "");
    mBaseSettings.setValue("database/login", "");
    mBaseSettings.setValue("database/password", "");
    mBaseSettings.sync();
}

/*Settings::~Settings()
{
    delete mpBaseSettings;
}*/

QString Settings::databaseName()
{
    return mBaseSettings.value("database/name", "").toString();
}

QString Settings::databaseHost()
{
    return mBaseSettings.value("database/host", "").toString();
}

QString Settings::databaseLogin()
{
    return mBaseSettings.value("database/login", "").toString();
}

QString Settings::databasePassword()
{
    return mBaseSettings.value("database/password", "").toString();
}

int Settings::databasePort()
{
    return mBaseSettings.value("database/port", "").toInt();
}

QString Settings::databaseLocalName()
{
    return QString("local");
}

QString Settings::databaseType()
{
    QString vType = mBaseSettings.value("database/type", "").toString().toUpper();
    if (vType == "POSTGRESQL")
    {
        return "QPSQL";
    }
    return "";
}

QString Settings::site()
{
    return mBaseSettings.value("site", "").toString();
}
