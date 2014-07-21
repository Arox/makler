#include "viewpostwidget.h"
#include "ui_viewpostwidget.h"
#include "globals.h"
#include "globalsbase.h"
#include "language.h"

#include <QFile>
#include <QFileInfo>
#include <QTime>
#include "messages.h"

ViewPostWidget::ViewPostWidget(int aUser_fk, bool aRemoveElement, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewPostWidget),
    mUser_fk(aUser_fk),
    mUsers(aRemoveElement, this),
    mFiles(aRemoveElement, this)
{
    ui->setupUi(this);
    ui->usersLayout->addWidget(&mUsers);
    ui->filesLayout->addWidget(&mFiles);

    connect(&mUsers, SIGNAL(removeElement()), this, SIGNAL(removeUser()));
    connect(&mFiles, SIGNAL(removeElement()), this, SIGNAL(removeFile()));
    connect(&mFiles, SIGNAL(clickLink(QString, QByteArray)), this, SIGNAL(clickLink(QString, QByteArray)));
}

ViewPostWidget::~ViewPostWidget()
{
    delete ui;
}

void ViewPostWidget::addUser(QString aName, int aId)
{
    mUsers.add(aName, aId);
}

void ViewPostWidget::addFile(QString aDisplayFile, QString aFileName)
{
    mFiles.add(aDisplayFile, aFileName);
}

void ViewPostWidget::save()
{
    int vCount = mUsers.countData();
    if (!vCount)
    {
        warning(this, TRANSLATE("Ошибка"), TRANSLATE("Должен быть хотя бы один адресат"));
    }
    qsrand(QTime::currentTime().msec());
    int vRnd = qrand() % 1000000;
    execQuery(QString("INSERT INTO messages (\"from\", message, date_create) VALUES (%1, '%2', now())")
              .arg(mUser_fk)
              .arg(vRnd));
    int vId = execQuery(QString("SELECT id FROM messages WHERE message = '%1' AND \"from\" = %2")
                        .arg(vRnd)
                        .arg(mUser_fk))[0]["id"].toInt();
    execQuery(QString("UPDATE messages SET message = E'%1' WHERE id = %2")
              .arg(ui->mpEdit->toPlainText().replace("'", "\\'"))
              .arg(vId));
    for (int i = 0; i < vCount; ++i)
    {
        int vIdUserTo = mUsers.data(i).toInt();
        execQuery(QString("INSERT INTO messages_to (\"to\", message_fk) VALUES(%1, %2)")
                  .arg(vIdUserTo)
                  .arg(vId));
    }
    vCount = mFiles.countData();
    for (int i = 0; i < vCount; ++i)
    {
        QString vNameFile = mFiles.data(i).toString();
        QFile vFile(vNameFile);

        if (!vFile.open(QIODevice::ReadOnly))
        {
            warning(this, TRANSLATE("невозможно добавить файл"), TRANSLATE("Ошибка при попытке открыт файл"));
            continue;
        }
        QFileInfo vInfoFile(vNameFile);
        QSqlQuery vQuery(QSqlDatabase::database("local"));
        vQuery.prepare("INSERT INTO files_for_message (message_fk, file, name) VALUES(:id, :file, :name)");
        vQuery.bindValue(":id", vId);
        vQuery.bindValue(":file", vFile.readAll().toBase64());
        vQuery.bindValue(":name", vInfoFile.fileName());
        vQuery.exec();
    }
}

void ViewPostWidget::load(int aIdMessage)
{
    ResponseType vResponse = execQuery(QString("SELECT message, \"from\" FROM messages WHERE id = %1").arg(aIdMessage));
    if (vResponse.count())
    {
        ResponseRecordType vRecord = vResponse[0];
        ui->mpEdit->setText(vRecord["message"].toString());
        ui->mpEdit->setReadOnly(true);
        QStringList vQuery;
        if (mUser_fk == vRecord["from"].toInt())
        {
            vQuery << "SELECT users.id as id, mans.name as name, mans.sername as sername, mans.patronymic as patronymic"
                   << "FROM messages_to"
                   << "INNER JOIN users ON users.id = messages_to.\"to\""
                   << "INNER JOIN mans ON mans.id = users.man_fk"
                   << QString("WHERE message_fk = %1").arg(aIdMessage);
        }
        else
        {
            vQuery << "SELECT users.id as id, mans.name as name, mans.sername as sername, mans.patronymic as patronymic"
                   << "FROM mans"
                   << "INNER JOIN users ON mans.id = users.man_fk"
                   << QString("WHERE users.id = %1").arg(aIdMessage);
        }
        ResponseType vToUsers = execQuery(vQuery.join(" "));
        foreach (ResponseRecordType vUser, vToUsers)
        {
            QString vName = QString("%1 %2 %3")
                    .arg(vUser["sername"].toString())
                    .arg(vUser["name"].toString())
                    .arg(vUser["patronymic"].toString());
            mUsers.add(vName, vUser["id"].toInt());
        }
    }

    ResponseType vFiles = execQuery(QString("SELECT file, name FROM files_for_message WHERE message_fk = %1").arg(aIdMessage));
    foreach (ResponseRecordType vFile, vFiles)
    {
        mFiles.add(vFile["name"].toString(), QByteArray::fromBase64(vFile["file"].toByteArray()));
    }
}

QList<int> ViewPostWidget::idUserList()
{
    QList<int> vIdUsers;
    int vCount = mUsers.countData();
    for (int i = 0; i < vCount; ++i)
    {
        int vId = mUsers.data(i).toInt();
        vIdUsers.append(vId);
    }
    return vIdUsers;
}
