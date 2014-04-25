#include "viewbases.h"
#include "ui_viewbases.h"
#include "globalsbase.h"
#include "dialoguniversal.h"
#include "appenddatabase.h"
#include "easysync.h"

ViewBases::ViewBases(QWidget *parent) :
    WidgetForControl(parent),
    ui(new Ui::ViewBases),
    mModel(this)
{
    ui->setupUi(this);
    mModel.load();
    ui->mpView->setModel(&mModel);
}

ViewBases::~ViewBases()
{
    delete ui;
}

void ViewBases::on_mpAdd_clicked()
{
    AppendDatabase* vpBaseEditor = new AppendDatabase(this);
    DialogUniversal vDialog(vpBaseEditor, TRANSLATE("Добавьте информацию о БД"),this);
    vDialog.exec();
    mModel.load();
    ui->mpView->setModel(0);
    ui->mpView->setModel(&mModel);
}

void ViewBases::on_mpRemove_clicked()
{
    int vRow = ui->mpView->currentIndex().row();
    if (vRow > -1)
    {
        int vId = mModel.id(vRow);
        execQuery(QString("DELETE FROM bases WHERE id = %1").arg(vId));
    }
}

void ViewBases::on_mpView_activated(const QModelIndex &index)
{
    Q_UNUSED(index);
}

QString ViewBases::name()
{
    return TRANSLATE("Синхронизация");
}

QSize ViewBases::minSize()
{
    return QSize(300, 370);
}

QSize ViewBases::maxSize()
{
    return QSize(300, 370);
}

QSize ViewBases::size()
{
    return QSize(300, 370);
}

QString ViewBases::idName()
{
    return "sync";
}

void ViewBases::on_mpView_clicked(const QModelIndex &index)
{
    if (index.row() > -1)
    {
        ui->mpSync->setEnabled(true);
        ui->mpRemove->setEnabled(true);
    }
    else
    {
        ui->mpSync->setEnabled(true);
        ui->mpRemove->setEnabled(true);
    }
}

void ViewBases::on_mpSync_clicked()
{
    if (ui->mpView->currentIndex().row() < 0) return;
    int vId = mModel.id(ui->mpView->currentIndex().row());
    ResponseType vBase = execQuery(QString("SELECT id, host, name, \"user\", port, pass FROM bases WHERE id = %1").arg(vId));
    if (!vBase.count()) return;
    QString vType = "QPSQL";
    QString vHost = vBase[0]["host"].toString();
    QString vName = vBase[0]["name"].toString();
    QString vLogin = vBase[0]["user"].toString();
    QString vPassword = vBase[0]["pass"].toString();
    int vPort = vBase[0]["port"].toInt();
    QSqlDatabase db = QSqlDatabase::addDatabase(vType, "source");
         db.setHostName(vHost);
         db.setDatabaseName(vName);
         db.setUserName(vLogin);
         db.setPassword(vPassword);
         db.setPort(vPort);
         db.open();
         qDebug() << db.lastError().text();
    QList< TableInfo > vTables;
    QList<QString> vFks;
    
    vTables << TableInfo("mans", "", "id", vFks);
    
    vFks << "man_fk";
    vTables << TableInfo("users", "", "id", vFks);
    vFks.clear();
    
    vFks << "user_fk";
    vTables << TableInfo("users_roles", "", "id", vFks);
    vFks.clear();
    
    vFks << "parent";
    vTables << TableInfo("address_city", "standart = FALSE", "id", vFks);
    vFks.clear();
    
    vFks << "city_fk";
    vTables << TableInfo("address_street", "standart = FALSE", "id", vFks);
    vFks.clear();

    vFks << "city_fk";
    vTables << TableInfo("address_microdistrict", "standart = FALSE", "id", vFks);
    vFks.clear();

    vFks << "street_fk" << "microdistrict_fk" << "object_fk";
    vTables << TableInfo("address", "", "id", vFks);
    vFks.clear();

    vFks << "agent_fk";
    vTables << TableInfo("objects", "", "id", vFks);
    vFks.clear();

    vFks << "object_fk";
    vTables << TableInfo("area", "", "id", vFks);
    vFks.clear();

    vFks << "id";
    vTables << TableInfo("aside", "", "id", vFks);
    vFks.clear();

    vFks << "object_fk";
    vTables << TableInfo("clientheader", "", "id", vFks);
    vFks.clear();

    vFks << "header_fk";
    vTables << TableInfo("clients", "", "id", vFks);
    vFks.clear();

    vFks << "from";
    vTables << TableInfo("messages", "", "id", vFks);
    vFks.clear();

    vFks << "to" << "message_fk";
    vTables << TableInfo("messages_to", "", "id", vFks);
    vFks.clear();

    vFks << "message_fk";
    vTables << TableInfo("files_for_message", "", "id", vFks);
    vFks.clear();

    vFks << "object_fk";
    vTables << TableInfo("orders", "", "id", vFks);
    vFks.clear();
    
    vFks << "object_in_order_fk" << "object_fk";
    vTables << TableInfo("objectstoorders", "", "id", vFks);
    vFks.clear();

    vFks << "object_fk";
    vTables << TableInfo("optionvalue", "", "id", vFks);
    vFks.clear();

    vFks << "object_fk";
    vTables << TableInfo("passport", "", "id", vFks);
    vFks.clear();

    vFks << "object_fk";
    vTables << TableInfo("photos", "", "id", vFks);
    vFks.clear();

    vFks << "object_fk";
    vTables << TableInfo("price", "", "id", vFks);
    vFks.clear();

    vTables << TableInfo("shoot", "", "id", vFks);
    vFks.clear();

    EasySyncTable vSync;
    vSync.sync("source", "local", vTables, vId);
}
