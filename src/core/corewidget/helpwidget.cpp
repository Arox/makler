#include "helpwidget.h"
#include "ui_helpwidget.h"

#include <QFile>
#include <QDir>

HelpWidget::HelpWidget(QWidget *parent) :
    MainWidget(parent),
    ui(new Ui::HelpWidget)
{
    ui->setupUi(this);
    QString vFileName = QString("%1/help/hallo.html").arg(qApp->applicationDirPath());
    QFile vFile(vFileName);
    if (vFile.open(QIODevice::ReadOnly))
    {
        ui->mpView->setText(vFile.readAll());
    }
}

HelpWidget::~HelpWidget()
{
    delete ui;
}

void HelpWidget::on_mpExit_clicked()
{
    emit back();
}
