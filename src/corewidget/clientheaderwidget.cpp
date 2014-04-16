#include "clientheaderwidget.h"
#include "ui_clientheaderwidget.h"

#include <QList>
#include <QMap>
#include <QVariant>
#include <QHBoxLayout>

#include "globalsbase.h"
#include "clientsphonewidget.h"

ClientHeaderWidget::ClientHeaderWidget(QWidget *parent) :
    MainWidget(parent),
    ui(new Ui::ClientHeaderWidget),
    mId(-1),
    mIdObjects(-1)
{
    ui->setupUi(this);
    mpContacts = new ClientsPhoneWidget(this);
    ui->mainLayout->insertWidget(0, mpContacts);
}

ClientHeaderWidget::~ClientHeaderWidget()
{
    delete ui;
}

void ClientHeaderWidget::setEnable(bool aEnable)
{
    ui->mpDescription->setEnabled(aEnable);
    ui->mpFirm->setEnabled(aEnable);
    //ui->mpGet->setEnabled(aEnable);
    mpContacts->setEnabled(aEnable);
}

void ClientHeaderWidget::load(int aAgent, int aIdObjects)
{
    MainWidget::load();

    mIdObjects = aIdObjects;
    mId = -1;
    ResponseType vQuery = execQuery(QString("SELECT get, firm, description, id FROM clientheader WHERE object_fk = %1").arg(mIdObjects));
    if (vQuery.count())
    {
        MainWidget::save();

        Q_ASSERT(aAgent >= 0);

        ui->mpFirm->setText(vQuery[0]["firm"].toString());

        ui->mpDescription->setText(vQuery[0]["description"].toString());
        mId = vQuery[0]["id"].toInt();
    }
    else
    {
        ui->mpFirm->setText("");
        ui->mpDescription->setText("");
    }
    mpContacts->load(mId);
    ResponseType vQueryAgent = execQuery(QString("SELECT \"name\", sername, patronymic FROM mans WHERE id = %1").arg(aAgent));
    if (vQueryAgent.count())
    {
        ui->mpAgent->setText(QString("%1 %2 %3")
                             .arg(vQueryAgent[0]["name"].toString())
                             .arg(vQueryAgent[0]["patronymic"].toString())
                             .arg(vQueryAgent[0]["sername"].toString()));
    }
}

void ClientHeaderWidget::save()
{
    if (!canSave()) return;
    if (mId >= 0)
    {
        execQuery(QString("UPDATE clientheader SET firm = '%1', description = '%2' WHERE id = %3")
                  .arg(ui->mpFirm->text())
                  .arg(ui->mpDescription->toPlainText())
                  .arg(mId));
    }
    else
    {
        execQuery(QString("INSERT INTO clientheader (object_fk) VALUES ('%1')").arg(mIdObjects));
        mId = execQuery(QString("SELECT id FROM clientheader WHERE object_fk = %1").arg(mIdObjects))[0]["id"].toInt();
        save();
    }
    mpContacts->save(mId);
    MainWidget::save();
    emit changed();
}

bool ClientHeaderWidget::canSave()
{
    return (ui->mpDescription->toPlainText().length() || ui->mpFirm->text().length() || mpContacts->canSave());
}
