#include "connecttodatabase.h"
#include "ui_connecttodatabase.h"

#include "settings.h"
#include "language.h"

#include "messages.h"
#include <QSqlDatabase>


ConnectToDatabase::ConnectToDatabase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectToDatabase)
  , mHost(TRANSLATE("Имя центрального компьютера"), MyLineEdit::Normal, this)
  , mName(TRANSLATE("Имя базы данных"), MyLineEdit::Normal, this)
  , mPort(TRANSLATE("Порт базы данных"), MyLineEdit::Normal, this)
  , mLogin(TRANSLATE("Имя пользователя базы данных"), MyLineEdit::Normal, this)
  , mPassword(TRANSLATE("Пароль пользователя базы данных"), MyLineEdit::Password, this)
{
    ui->setupUi(this);
    ui->verticalLayout_2->insertWidget(0, &mHost);
    ui->verticalLayout_2->insertWidget(1, &mName);
    ui->verticalLayout_2->insertWidget(2, &mPort);
    ui->verticalLayout_2->insertWidget(3, &mLogin);
    ui->verticalLayout_2->insertWidget(4, &mPassword);
}

ConnectToDatabase::~ConnectToDatabase()
{
    delete ui;
}

void ConnectToDatabase::setHost(QString aHost)
{
    mHost.setRealText(aHost);
}

QString ConnectToDatabase::host()
{
    return mHost.text();
}

void ConnectToDatabase::setName(QString aName)
{
    mName.setRealText(aName);
}

QString ConnectToDatabase::name()
{
    return mName.text();
}

void ConnectToDatabase::setPort(int aPort)
{
    mPort.setRealText(QString("%1").arg(aPort));
}

int ConnectToDatabase::port()
{
    bool ok;
    int vResult = mPort.text().toInt(&ok);
    if (ok) return vResult; else return -1;
}

void ConnectToDatabase::setLogin(QString aLogin)
{
    mLogin.setRealText(aLogin);
}

QString ConnectToDatabase::login()
{
    return mLogin.text();
}

void ConnectToDatabase::setPassword(QString aPassword)
{
    mPassword.setRealText(aPassword);
}

QString ConnectToDatabase::password()
{
    return mPassword.text();
}

bool ConnectToDatabase::isConnect()
{
    QString vType = Settings::databaseType();
    QString vHost = host();
    QString vName = name();
    QString vLogin = login();
    QString vPassword = password();
    int vPort = port();
    QSqlDatabase db = QSqlDatabase::addDatabase(vType, "try");
         db.setHostName(vHost);
         db.setDatabaseName(vName);
         db.setUserName(vLogin);
         db.setPassword(vPassword);
         db.setPort(vPort);
    return db.open();
}

void ConnectToDatabase::readSettings()
{
    setHost(Settings::databaseHost());
    setName(Settings::databaseName());
    setPort(Settings::databasePort());
    setLogin(Settings::databaseLogin());
    setPassword(Settings::databasePassword());
}

void ConnectToDatabase::saveSettings()
{
    Settings::setDatabaseHost(host());
    Settings::setDatabaseName(name());
    Settings::setDatabasePort(port());
    Settings::setDatabaseLogin(login());
    Settings::setDatabasePassword(password());
}

void ConnectToDatabase::showisConnect()
{
    if (!isConnect())
    {
        warning(this, TRANSLATE("Ошибка соединения"), TRANSLATE("Не удалось соедениться с БД"));
    }
    else
    {
        information(this, TRANSLATE("Соединение успешно"), TRANSLATE("Соединение с базой данных прошло успешно"));
    }
}
