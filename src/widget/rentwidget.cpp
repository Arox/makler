#include "rentwidget.h"
#include "ui_rentwidget.h"

#include <QTime>
#include <QMessageBox>

#include "globalsbase.h"
#include "globals.h"
#include "language.h"

RentWidget::RentWidget(int aAgent, QWidget *parent) :
    GeneralWidget(parent),
    ui(new Ui::RentWidget),
    mId(-1),
    mAgent(aAgent),
    mIsLoad(false),
    mAddress(AddressWidget::NORMAL),
    mHomeAddress(AddressHomeWidget::NORMAL)
{
    ui->setupUi(this);
    ui->mainLayout->addWidget(&mClient);
    ui->mainLayout->addWidget(&mType);
    ui->mainLayout->addWidget(&mAddress);
    ui->mainLayout->addWidget(&mHomeAddress);
    ui->mainLayout->addWidget(&mArea);
    ui->mainLayout->addWidget(&mPrice);
    ui->mainLayout->addWidget(&mInformation);
    ui->mainLayout->addWidget(&mComment);
    ui->mainLayout->addWidget(&mButtons);

    mInformation.setNameDictionary("rent", 4);
    connect(&mButtons, SIGNAL(back()), this, SLOT(backWidget()));
    connect(&mButtons, SIGNAL(noSave()), this, SLOT(noSave()));
}

RentWidget::~RentWidget()
{
    delete ui;
}

void RentWidget::backWidget()
{
    if (mClient.canSave() && (mHomeAddress.canSave() || mAddress.canSave()))
    {

        mClient.save();
        mInformation.save();
        mComment.save();
        mType.save();
        mArea.save();
        mPrice.save();
        mAddress.save();
        mHomeAddress.save();
        emit back(this);
    }
    else
    {
        QMessageBox::warning(this, TRANSLATE("Не все поля заполнены"), TRANSLATE("Адрес объекта, телефон и имя клиента обязательны для заполнения"));
    }

}

void RentWidget::noSave()
{
    if (!mIsLoad)
    {
        execQuery(QString("DELETE FROM objects WHERE id = %1").arg(mId));
    }
    emit back(this);
}

void RentWidget::load(int aId)
{
    mId = aId;

    if (mId < 0)
    {
        int vTypeId = execQuery(QString("SELECT id FROM types WHERE type = 'rent'"))[0]["id"].toInt();
        qsrand(QTime::currentTime().msec());
        int vComment = qrand() % 10000000;
        execQuery(QString("insert into objects (type_fk, \"create\", comment, agent_fk) values (%1, now(), '%2', %3)")
                  .arg(vTypeId)
                  .arg(vComment)
                  .arg(mAgent));
        mId = execQuery(QString("SELECT id FROM objects WHERE type_fk = %1 AND comment = '%2'")
                        .arg(vTypeId)
                        .arg(vComment))[0]["id"].toInt();
        execQuery(QString("UPDATE objects SET comment = '' WHERE id = %1")
                  .arg(mId));

    }
    else
    {
        mIsLoad = true;
    }
    int vAgent = execQuery(QString("SELECT agent_fk FROM objects WHERE id = %1")
                                 .arg(mId))[0]["agent_fk"].toInt();


    mClient.load(mAgent, mId);
    mInformation.load(mId);
    mComment.load(mId);
    mType.load(mId);
    mArea.load(mId);
    mPrice.load(mId);
    mAddress.load(mId, 1);
    mHomeAddress.load(mId, 1);
    mButtons.setId(mId);

    mClient.setEnabled(vAgent == mAgent);
    mInformation.setEnabled(vAgent == mAgent);
    mComment.setEnabled(vAgent == mAgent);
    mType.setEnabled(vAgent == mAgent);
    mArea.setEnabled(vAgent == mAgent);
    mPrice.setEnabled(vAgent == mAgent);
    mAddress.setEnabled(vAgent == mAgent);
    mHomeAddress.setEnabled(vAgent == mAgent);



    ResponseRecordType vRecord = execQuery(QString("SELECT \"create\", now() as read FROM objects WHERE id = %1")
                    .arg(mId))[0];
    ui->mpDateCreate->setText(vRecord["create"].toDate().toString(DATEFORMAT));
    ui->mpDateRead->setText(vRecord["read"].toDate().toString(DATEFORMAT));
    ui->mpNumber->setText(QString(TRANSLATE("№%1")).arg(mId));
}

QString RentWidget::name()
{
    return TRANSLATE("Аренда");
}

QSize RentWidget::minSize()
{
    return QSize(0, 0);
}

QSize RentWidget::size()
{
    return QSize(864, 837);
}

QSize RentWidget::maxSize()
{
    return QSize(0, 0);
}

QString RentWidget::idName()
{
    return "rent";
}
