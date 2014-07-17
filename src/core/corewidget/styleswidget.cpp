#include "styleswidget.h"
#include "ui_styleswidget.h"

#include "styles.h"

StylesWidget::StylesWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StylesWidget)
{
    ui->setupUi(this);
    ui->mpStyles->addItems(Styles::styles());
    connect(ui->mpStyles, SIGNAL(activated(QString)), this, SLOT(changeStyle(QString)));
}

StylesWidget::~StylesWidget()
{
    delete ui;
}

void StylesWidget::changeStyle(QString aNameStyle)
{
    QString vStyle = Styles::style(aNameStyle);
    ui->widget->setStyleSheet(vStyle);
    /*
    ui->lineEdit->setStyleSheet(vStyle);
    ui->checkBox->setStyleSheet(vStyle);
    ui->mpStyles->setStyleSheet(vStyle);
    ui->radioButton->setStyleSheet(vStyle);
    ui->pushButton->setStyleSheet(vStyle);
    ui->plainTextEdit->setStyleSheet(vStyle);*/
}

void StylesWidget::on_mpOk_clicked()
{
    Styles::setStyle(ui->mpStyles->currentText());
}

void StylesWidget::on_mpCancel_clicked()
{
    ui->mpStyles->setCurrentText(Styles::currentStyleName());
    changeStyle(Styles::currentStyleName());
}
