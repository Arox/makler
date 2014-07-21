#include "buttonswidget.h"
#include "ui_buttonswidget.h"

#include "dialogphotoalbum.h"
#include "passportwidget.h"
#include "dialoguniversal.h"
#include "language.h"
#include "settings.h"

#include <QDesktopServices>
#include <QUrl>

ButtonsWidget::ButtonsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ButtonsWidget),
    mIdObject(-1)
{
    ui->setupUi(this);
    ui->mpBack->setProperty("color", "true");
}

ButtonsWidget::~ButtonsWidget()
{
    delete ui;
}

void ButtonsWidget::on_mpPhoto_clicked()
{
    Q_ASSERT(mIdObject > -1);
    DialogPhotoAlbum vAlbum(mIdObject, this);
    vAlbum.setChangePhotoEnabled(isChangeEnabled());
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
    vWidget.setEnabled(isChangeEnabled());
    vWidget.load(mIdObject);
    DialogUniversal vDialog(&vWidget, TRANSLATE("Договор"), this);
    vDialog.exec();
    vWidget.setParent(this);
}

void ButtonsWidget::on_mpNoSave_clicked()
{
    emit noSave();
}

void ButtonsWidget::setChangeEnabled(bool aEnabled)
{
    mEnabled = aEnabled;
}

bool ButtonsWidget::isChangeEnabled() const
{
    return mEnabled;
}
