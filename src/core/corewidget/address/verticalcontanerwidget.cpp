#include "verticalcontanerwidget.h"
#include "ui_verticalcontanerwidget.h"

VerticalContanerWidget::VerticalContanerWidget(QString vText, LocationSelect *apSelect, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VerticalContanerWidget)
  ,mpData(apSelect)
{
    ui->setupUi(this);
    QLabel* vpLabel = new QLabel(vText);
    vpLabel->setAlignment(Qt::AlignHCenter);
    ui->mainLayout->addWidget(vpLabel);
    ui->mainLayout->addWidget(apSelect);
}

VerticalContanerWidget::~VerticalContanerWidget()
{
    delete ui;
}

LocationSelect* VerticalContanerWidget::data() const
{
    return mpData;
}
