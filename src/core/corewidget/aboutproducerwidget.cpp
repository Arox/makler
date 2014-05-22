#include "aboutproducerwidget.h"
#include "ui_aboutproducerwidget.h"

AboutProducerWidget::AboutProducerWidget(QWidget *parent) :
    MainWidget(parent),
    ui(new Ui::AboutProducerWidget)
{
    ui->setupUi(this);
    ui->mpVersion->setText(QString("%1 %2").arg(ui->mpVersion->text()).arg(qApp->applicationVersion()));
}

AboutProducerWidget::~AboutProducerWidget()
{
    delete ui;
}

void AboutProducerWidget::on_mpExit_clicked()
{
    emit back();
}
