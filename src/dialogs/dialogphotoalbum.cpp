#include "dialogphotoalbum.h"
#include "ui_dialogphotoalbum.h"

DialogPhotoAlbum::DialogPhotoAlbum(int aIdObject, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPhotoAlbum),
    mPhotoAlbum(aIdObject)
{
    ui->setupUi(this);
    ui->mainLayout->addWidget(&mPhotoAlbum);
}

DialogPhotoAlbum::~DialogPhotoAlbum()
{
    delete ui;
}

void DialogPhotoAlbum::setChangePhotoEnabled(bool aEnabled)
{
    mPhotoAlbum.setEnabledChangePhoto(aEnabled);
}
