#include "commentwidget.h"
#include "ui_commentwidget.h"

#include "globalsbase.h"

CommentWidget::CommentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommentWidget)
  ,mIdObject(-1)
{
    ui->setupUi(this);
    ui->label->setProperty("title", "true");
    //connect(ui->mpComment, SIGNAL(textChanged()), this, SLOT(save()));
}

CommentWidget::~CommentWidget()
{
    delete ui;
}

void CommentWidget::load(int aIdObject)
{
    mIdObject = aIdObject;
    ResponseType vResponse = execQuery(
                QString("SELECT comment FROM objects WHERE id = %1")
                                   .arg(mIdObject));
    ui->mpComment->setPlainText("");
    if (vResponse.count())
    {
        ui->mpComment->appendPlainText(vResponse[0]["comment"].toString());
    }

}

void CommentWidget::save()
{
    if (mIdObject >= 0)
    {
        execQuery(QString("UPDATE objects SET comment = '%1' WHERE id = %2")
                  .arg(ui->mpComment->toPlainText())
                  .arg(mIdObject));
    }
}
