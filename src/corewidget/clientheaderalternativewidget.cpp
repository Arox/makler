#include "clientheaderalternativewidget.h"
#include "ui_clientheaderalternativewidget.h"
#include "globalsbase.h"
#include "dialoguniversal.h"
#include "passportwidget.h"

ClientHeaderAlternativeWidget::ClientHeaderAlternativeWidget(QWidget *parent) :
    MainWidget(parent),
    ui(new Ui::ClientHeaderAlternativeWidget),
    mId(-1),
    mIdObjects(-1)
{
    ui->setupUi(this);


    mNames << ui->mpFIO << ui->mpFIO2 << ui->mpFIO3 << ui->mpFIO4 << ui->mpFIO5;
    mPhones << ui->mpPhone << ui->mpPhone2 << ui->mpPhone3 << ui->mpPhone4 << ui->mpPhone5;

    for (int i = 0; i < mNames.count(); ++i)
    {
        connect(mNames.at(i), SIGNAL(textChanged(QString)), &mMapper, SLOT(map()));
        connect(mPhones.at(i), SIGNAL(textChanged(QString)), &mMapper, SLOT(map()));
        mMapper.setMapping(mNames.at(i), i);
        mMapper.setMapping(mPhones.at(i), i);
        mIdClients[i] = -1;
    }
    connect(&mMapper, SIGNAL(mapped(int)), this, SLOT(saveClient(int)));
    connect(ui->mpDescription, SIGNAL(textChanged()), this, SLOT(save()));
}

ClientHeaderAlternativeWidget::~ClientHeaderAlternativeWidget()
{
    delete ui;
}

void ClientHeaderAlternativeWidget::load(int aAgent, int aIdObjects)
{
    Q_UNUSED(aAgent);
    MainWidget::load();

    for (int i = 0; i < mNames.count(); ++i)
    {
        mNames.at(i)->blockSignals(true);
        mNames.at(i)->setText("");
        mNames.at(i)->blockSignals(false);
        mPhones.at(i)->blockSignals(true);
        mPhones.at(i)->setText("");
        mPhones.at(i)->blockSignals(false);
    }

    mIdObjects = aIdObjects;
    mId = -1;
    for (int i = 0; i < mNames.count(); ++i) mIdClients[i] = -1;
    ResponseType vQuery = execQuery(QString("SELECT get, firm, description, id FROM clientheader WHERE object_fk = %1").arg(mIdObjects));
    if (vQuery.count())
    {
        MainWidget::save();

        Q_ASSERT(aAgent >= 0);

        ui->mpDescription->blockSignals(true);
        ui->mpDescription->setPlainText(vQuery[0]["description"].toString());
        ui->mpDescription->blockSignals(false);
        mId = vQuery[0]["id"].toInt();

        ResponseType vQueryClients = execQuery(QString("SELECT id, fio, phone FROM clients WHERE header_fk = %1").arg(mId));
        int i = 0;
        ResponseRecordType vRecord;
        foreach (vRecord, vQueryClients)
        {
            mNames.at(i)->blockSignals(true);
            mPhones.at(i)->blockSignals(true);
            mIdClients[i] = vQueryClients[i]["id"].toInt();
            mNames[i]->setText(vQueryClients[i]["fio"].toString());
            mPhones[i]->setText(vQueryClients[i]["phone"].toString());
            mNames.at(i)->blockSignals(false);
            mPhones.at(i)->blockSignals(false);
            ++i;
        }
    }
    else
    {
        ui->mpDescription->blockSignals(true);
        ui->mpDescription->setPlainText("");
        ui->mpDescription->blockSignals(false);
        for (int i = 0; i < mNames.count(); ++i)
        {
            mNames.at(i)->blockSignals(true);
            mPhones.at(i)->blockSignals(true);
            mNames.at(i)->setText("");
            mPhones.at(i)->setText("");
            mNames.at(i)->blockSignals(false);
            mPhones.at(i)->blockSignals(false);
        }
    }
}

void ClientHeaderAlternativeWidget::saveClient(int aNumber)
{
    QString vFio = mNames.at(aNumber)->text();
    QString vPhone = mPhones.at(aNumber)->text();

    if (vFio.isEmpty() || vPhone.isEmpty()) return;
    if (mId < 0)
    {
        save();
    }

    if (mIdClients[aNumber] < 0)
    {
        execQuery(QString("INSERT INTO clients (header_fk, fio, phone) VALUES (%1, '%2', '%3')")
                  .arg(mId)
                  .arg(vFio)
                  .arg(vPhone));
        mIdClients[aNumber] = execQuery(QString("SELECT id FROM clients WHERE header_fk = %1 AND fio = '%2' AND phone = '%3' ORDER BY id DESC LIMIT 1")
                                        .arg(mId)
                                        .arg(vFio)
                                        .arg(vPhone))[0]["id"].toInt();
    }
    else
    {
        execQuery(QString("UPDATE clients SET fio = '%1', phone = '%2' WHERE id = %3")
                  .arg(vFio)
                  .arg(vPhone)
                  .arg(mIdClients[aNumber]));
    }
    emit changed();
}

void ClientHeaderAlternativeWidget::save()
{
    if (mId >= 0)
    {
        execQuery(QString("UPDATE clientheader SET description = '%1' WHERE id = %2")
                  .arg(ui->mpDescription->toPlainText())
                  .arg(mId));
    }
    else
    {
        execQuery(QString("INSERT INTO clientheader (object_fk) VALUES ('%1')").arg(mIdObjects));
        mId = execQuery(QString("SELECT id FROM clientheader WHERE object_fk = %1").arg(mIdObjects))[0]["id"].toInt();
        save();
    }
    MainWidget::save();
    emit changed();
}

void ClientHeaderAlternativeWidget::on_mpDiler_clicked()
{
    PassportWidget vWidget;
    vWidget.load(mIdObjects);
    DialogUniversal vDialog(&vWidget, TRANSLATE("Договор"), this);
    vDialog.exec();
    vWidget.setParent(this);
}
