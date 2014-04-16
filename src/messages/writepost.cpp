#include "writepost.h"
#include "ui_writepost.h"

#include "globalsbase.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTime>

WritePost::WritePost(int aUser_fk, QWidget *parent) :
    MainWidget(parent),
    ui(new Ui::WritePost),
    mViewPost(aUser_fk, true, this),
    mUser_fk(aUser_fk)
{
    ui->setupUi(this);

    loadUsers();
    ui->verticalLayout->insertWidget(1, &mViewPost);
    connect(&mViewPost, SIGNAL(removeUser()), this, SLOT(loadUsers()));
}

WritePost::~WritePost()
{
    delete ui;
}

void WritePost::on_mpAddFile_clicked()
{
    QString vFileName = QFileDialog::getOpenFileName(this, TRANSLATE("Выбор файла"), TRANSLATE("Выберите файл"));
    if (vFileName.isEmpty()) return;
    QFile vFile(vFileName);
    if (!vFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, TRANSLATE("невозможно добавить файл"), TRANSLATE("Ошибка при попытке открыт файл"));
    }
    QFileInfo vInfo(vFileName);
    mViewPost.addFile(vInfo.baseName(), vFileName);
}

void WritePost::on_mpUsers_activated(int index)
{
    mViewPost.addUser(ui->mpUsers->itemText(index), ui->mpUsers->itemData(index).toInt());
    ui->mpUsers->removeItem(index);
}

void WritePost::loadUsers()
{
    ui->mpUsers->clear();
    ResponseType vResponse = execQuery(QString("SELECT name as name, sername as sername, patronymic as patronymic, users.id as id FROM users INNER JOIN mans ON users.man_fk = mans.id WHERE users.is_active = TRUE"));
    QList<int> vIdUsers = mViewPost.idUserList();

    for (int i = 0; i < vResponse.count(); ++i)
    {
        int vId = vResponse[i]["id"].toInt();
        if (vIdUsers.contains(vId))
        {
            continue;
        }
        QString vName = QString("%1 %2 %3")
                .arg(vResponse[i]["sername"].toString())
                .arg(vResponse[i]["name"].toString())
                .arg(vResponse[i]["patronymic"].toString());
        ui->mpUsers->addItem(vName, vResponse[i]["id"]);

    }
}

void WritePost::on_mpSend_clicked()
{
    mViewPost.save();
    emit back();
}

