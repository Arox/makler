#include "horizontalcontainerwidget.h"
#include "ui_horizontalcontainerwidget.h"

HorizontalContainerWidget::HorizontalContainerWidget(QString vText, LocationSelect *apSelect, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HorizontalContainerWidget)
  ,mpData(apSelect)
{
    ui->setupUi(this);
    QLabel* vpLabel = new QLabel(vText);
    vpLabel->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    ui->horizontalLayout->addWidget(vpLabel);
    ui->horizontalLayout->addWidget(apSelect);
}

HorizontalContainerWidget::~HorizontalContainerWidget()
{
    delete ui;
}

LocationSelect* HorizontalContainerWidget::data() const
{
    return mpData;
}
