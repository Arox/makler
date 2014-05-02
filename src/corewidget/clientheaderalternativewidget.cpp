#include "clientheaderalternativewidget.h"
#include "ui_clientheaderalternativewidget.h"
#include "globalsbase.h"
#include "language.h"
#include "dialoguniversal.h"
#include "passportwidget.h"

ClientHeaderAlternativeWidget::ClientHeaderAlternativeWidget(QWidget *parent) :
    MainWidget(parent),
    ui(new Ui::ClientHeaderAlternativeWidget),
    mId(-1),
    mIdObjects(-1)
{
    ui->setupUi(this);

    mpContact = new ClientsPhoneWidget(this);
    ui->verticalLayout->insertWidget(0, mpContact);

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

    mIdObjects = aIdObjects;
    mId = -1;
    ResponseType vQuery = execQuery(QString("SELECT get, firm, description, id FROM clientheader WHERE object_fk = %1").arg(mIdObjects));
    if (vQuery.count())
    {
        MainWidget::save();

        Q_ASSERT(aAgent >= 0);

        ui->mpDescription->blockSignals(true);
        ui->mpDescription->setPlainText(vQuery[0]["description"].toString());
        ui->mpDescription->blockSignals(false);
        mId = vQuery[0]["id"].toInt();
    }
    else
    {
        ui->mpDescription->blockSignals(true);
        ui->mpDescription->setPlainText("");
        ui->mpDescription->blockSignals(false);
    }
    mpContact->load(mId);
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
    mpContact->save(mId);
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
