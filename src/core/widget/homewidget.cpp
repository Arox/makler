#include "homewidget.h"
#include "ui_homewidget.h"

#include <QTime>
#include "messages.h"

#include "globalsbase.h"
#include "globals.h"
#include "language.h"

HomeWidget::HomeWidget(int aAgent, QWidget *parent) :
    GeneralWidget(parent),
    ui(new Ui::HomeWidget),
    mId(-1),
    mAgent(aAgent),
    mIsLoad(false),
    mAddress(location::NORMAL)
{
    ui->setupUi(this);
    ui->mainLayout->addWidget(&mClient);
    ui->mainLayout->addWidget(&mType);
    ui->mainLayout->addWidget(&mAddress);
    ui->mainLayout->addWidget(&mArea);
    ui->mainLayout->addWidget(&mPrice);
    ui->mainLayout->addWidget(&mInformation);
    ui->mainLayout->addWidget(&mComment);
    ui->mainLayout_2->insertWidget(3, &mButtons);

    mInformation.setNameDictionary("home", 5);
    connect(&mButtons, SIGNAL(back()), this, SLOT(backWidget()));
    connect(&mButtons, SIGNAL(noSave()), this, SLOT(noSave()));
}

void HomeWidget::backWidget()
{
    if (mClient.canSave() && mAddress.canSave())
    {
        mClient.save();
        mInformation.save();
        mComment.save();
        mType.save();
        mArea.save();
        mPrice.save();
        mAddress.save();
        emit back(this);
    }
    else
    {
        warning(this, TRANSLATE("Не все поля заполнены"), TRANSLATE("Адрес объекта, телефон и имя клиента обязательны для заполнения"));
    }
}

void HomeWidget::noSave()
{
    if (!mIsLoad)
    {
        execQuery(QString("DELETE FROM objects WHERE id = %1").arg(mId));
    }
    emit back(this);
}

HomeWidget::~HomeWidget()
{
    delete ui;
}

void HomeWidget::load(int aId)
{
    mId = aId;


    if (mId < 0)
    {
        int vTypeId = execQuery(QString("SELECT id FROM types WHERE type = 'home'"))[0]["id"].toInt();
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
        mIsLoad = false;

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
    mAddress.load(mId);
    mButtons.setId(mId);

    mClient.setEnabled(vAgent == mAgent);
    mInformation.setEnabled(vAgent == mAgent);
    mComment.setEnabled(vAgent == mAgent);
    mType.setEnabled(vAgent == mAgent);
    mArea.setEnabled(vAgent == mAgent);
    mPrice.setEnabled(vAgent == mAgent);
    mAddress.setEnabled(vAgent == mAgent);
    mButtons.setChangeEnabled(vAgent == mAgent);

    ResponseRecordType vRecord = execQuery(QString("SELECT \"create\", now() as read FROM objects WHERE id = %1")
                    .arg(mId))[0];
    ui->mpDateCreate->setText(vRecord["create"].toDate().toString(DATEFORMAT));
    ui->mpDateRead->setText(vRecord["read"].toDate().toString(DATEFORMAT));
    ui->mpNumber->setText(QString(TRANSLATE("№%1")).arg(mId));
}

QString HomeWidget::name()
{
    return TRANSLATE("Домовладения");
}

QSize HomeWidget::minSize()
{
    return QSize(0, 0);
}

QSize HomeWidget::size()
{
    return QSize(864, 762);
}

QSize HomeWidget::maxSize()
{
    return QSize(0, 0);
}

QString HomeWidget::idName()
{
    return "home";
}
