#include "clientsphonewidget.h"
#include "ui_clientsphonewidget.h"

#include <QHBoxLayout>
#include "globalsbase.h"

ClientsPhoneWidget::ClientsPhoneWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientsPhoneWidget)
{
    ui->setupUi(this);

    mpArea = new QScrollArea(this);
    mpArea->setBackgroundRole(QPalette::NoRole);
    mpArea->setWidget(ui->mpMainWidget);
    mpArea->setWidgetResizable(true);

    ui->horizontalLayout->addWidget(mpArea);

    ui->mpAdd->setObjectName("addcontactbutton");
    connect(ui->mpAdd, SIGNAL(clicked()), this, SLOT(addContact()));
    connect(&mDispatcher, SIGNAL(mapped(QWidget*)), this, SLOT(removeContact(QWidget*)));
}

ClientsPhoneWidget::~ClientsPhoneWidget()
{
    delete ui;
}

void ClientsPhoneWidget::addContact()
{
    QHBoxLayout* vpLayout = new QHBoxLayout();
    MyLineEdit* vpFio = new MyLineEdit(TRANSLATE("ФИО"), MyLineEdit::Normal, this);
    MyLineEdit* vpPhone = new MyLineEdit(TRANSLATE("Телефон"), MyLineEdit::Normal, this);
    QPushButton* vpButton = new QPushButton(this);
    mpFios.append(vpFio);
    mpPhones.append(vpPhone);
    mIds.append(-1);
    vpLayout->addWidget(vpFio);
    vpLayout->addWidget(vpPhone);
    vpLayout->addWidget(vpButton);
    mContainers.append(vpLayout);

    vpButton->setObjectName("deletecontactbutton");

    connect(vpButton, SIGNAL(clicked()), &mDispatcher, SLOT(map()));
    mDispatcher.setMapping(vpButton, vpFio);

    ui->centralLayout->insertLayout(0 , vpLayout);
}

void ClientsPhoneWidget::load(int aIdHeader)
{
    while (mIds.count() > 0)
    {
        delete mpFios[0];
        delete mpPhones[0];
        mpFios.removeAt(0);
        mpPhones.removeAt(0);
        mIds.removeAt(0);
    }
    if (aIdHeader >= 0)
    {
        ResponseType vQueryClients = execQuery(QString("SELECT id, fio, phone FROM clients WHERE header_fk = %1").arg(aIdHeader));
        ResponseRecordType vRecord;
        int i = 0;
        foreach (vRecord, vQueryClients)
        {
            mIds.append(vQueryClients[i]["id"].toInt());
            addContact();
            mpFios[i]->setText(vQueryClients[i]["fio"].toString());
            mpPhones[i]->setText(vQueryClients[i]["phone"].toString());
            ++i;
        }
    }
    if (mpFios.count() == 0)
    {
        addContact();
    }
}

void ClientsPhoneWidget::saveClient(int aNumber, int aIdHeader)
{
    Q_ASSERT(aNumber < mpFios.count());

    QString vFio = mpFios[aNumber]->text();
    QString vPhone = mpPhones[aNumber]->text();

    if (!vFio.length() && !vPhone.length()) return;
    if (mIds[aNumber] < 0)
    {
        execQuery(QString("INSERT INTO clients (header_fk, fio, phone) VALUES (%1, '%2', '%3')")
                  .arg(aIdHeader)
                  .arg(vFio)
                  .arg(vPhone));
        mIds[aNumber] = execQuery(QString("SELECT id FROM clients WHERE header_fk = %1 AND fio = '%2' AND phone = '%3' ORDER BY id DESC LIMIT 1")
                                        .arg(aIdHeader)
                                        .arg(vFio)
                                        .arg(vPhone))[0]["id"].toInt();
    }
    else
    {
        execQuery(QString("UPDATE clients SET fio = '%1', phone = '%2' WHERE id = %3")
                  .arg(vFio)
                  .arg(vPhone)
                  .arg(mIds[aNumber]));
    }

}

void ClientsPhoneWidget::save(int aIdHeader)
{
    if (aIdHeader < 0)
    {
        return;
    }

    for (int i = 0; i < mpFios.count(); ++i)
    {
        saveClient(i, aIdHeader);
    }

}

bool ClientsPhoneWidget::canSave()
{
    bool vResult = false;
    for (int i = 0; i < mpFios.count(); ++i)
    {
        if (mpFios[i]->text().length() || mpPhones[i]->text().length())
        {
            vResult = true;
            break;
        }
    }
    return vResult;
}

void ClientsPhoneWidget::removeContact(QWidget *apRemoveObject)
{
    int vIndex = -1;
    MyLineEdit* vpFio = dynamic_cast<MyLineEdit*>(apRemoveObject);
    if (!vpFio) return;
    for (int i = 0; i < mpFios.count(); ++i)
    {
        if (mpFios[i] == vpFio)
        {
            vIndex = i;
            break;
        }
    }
    if (vIndex < 0) return;

    if (mContainers[vIndex]->indexOf(mpFios[vIndex]) >= 0)
    {
        QLayout* vpLayout = mContainers[vIndex];
        int i = 0;
        while (vpLayout->count()) {
            if (vpLayout->itemAt(i)->widget())
            {
                delete vpLayout->itemAt(0)->widget();
            }
            else
            {
                i++;
            }
        }
        delete vpLayout;
    }

    mpFios.removeAt(vIndex);
    mpPhones.removeAt(vIndex);
    mIds.removeAt(vIndex);
    mContainers.removeAt(vIndex);
}
