#include "buttonswidget.h"
#include "ui_buttonswidget.h"

#include "dialogphotoalbum.h"
#include "passportwidget.h"
#include "dialoguniversal.h"
#include "globalsbase.h"
#include "settings.h"

#include <QDesktopServices>
#include <QUrl>

ButtonsWidget::ButtonsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ButtonsWidget),
    mIdObject(-1)
{
    ui->setupUi(this);
}

ButtonsWidget::~ButtonsWidget()
{
    delete ui;
}

void ButtonsWidget::on_mpPhoto_clicked()
{
    Q_ASSERT(mIdObject > -1);
    DialogPhotoAlbum vAlbum(mIdObject, this);
    vAlbum.exec();
}

void ButtonsWidget::setId(int aIdObject)
{
    mIdObject = aIdObject;
}

void ButtonsWidget::on_mpBack_clicked()
{
    emit back();
}

void ButtonsWidget::on_mpOrder_clicked()
{
    PassportWidget vWidget;
    vWidget.load(mIdObject);
    DialogUniversal vDialog(&vWidget, TRANSLATE("Договор"), this);
    vDialog.exec();
    vWidget.setParent(this);
}

void ButtonsWidget::on_mpNoSave_clicked()
{
    emit noSave();
}
