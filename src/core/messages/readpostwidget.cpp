#include "readpostwidget.h"
#include "ui_readpostwidget.h"
#include "globalsbase.h"
#include "language.h"

#include <QFileDialog>
#include <QFile>
#include "messages.h"

ReadPostWidget::ReadPostWidget(int aIdUser, int aIdMessage, QWidget *parent) :
    MainWidget(parent),
    ui(new Ui::ReadPostWidget),
    mView(aIdUser, false, this)
{
    ui->setupUi(this);
    mView.load(aIdMessage);
    ui->mainLayout->addWidget(&mView);
    execQuery(QString("UPDATE messages SET readed = true WHERE id = %1").arg(aIdMessage));
    connect(&mView, SIGNAL(clickLink(QString, QByteArray)), this, SLOT(saveFile(QString, QByteArray)));
}

ReadPostWidget::~ReadPostWidget()
{
    delete ui;
}

void ReadPostWidget::saveFile(QString aName, QByteArray aArray)
{
    QString vName = QFileDialog::getSaveFileName(this, TRANSLATE("Укажите имя файла и место для сохранения"),aName );
    if (vName.length())
    {
        QFile vFile(vName);
        if (!vFile.open(QIODevice::WriteOnly))
        {
            warning(this, TRANSLATE("ошибка записи файла"), TRANSLATE("Не удалось записать файл на диск"));
            return;
        }
        vFile.write(aArray);
    }
}

void ReadPostWidget::on_mpClose_clicked()
{
    emit back();
}
