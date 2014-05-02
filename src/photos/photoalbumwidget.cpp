#include "photoalbumwidget.h"
#include "ui_photoalbumwidget.h"

#include <QDebug>
#include <QFileDialog>
#include "language.h"

PhotoAlbumWidget::PhotoAlbumWidget(int aIdObject, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PhotoAlbumWidget)
{
    ui->setupUi(this);

    float vKoff = 0.27;
    ui->mpBack->setMaximumSize(20, height() * vKoff);
    ui->mpBack->setMinimumSize(20, height() * vKoff);

    ui->mpNext->setMaximumSize(20, height() * vKoff);
    ui->mpNext->setMinimumSize(20, height() * vKoff);

    mpGallery = new GalleryGrapicsScene(aIdObject, 0,0, ui->mpGallery->width(), ui->mpGallery->height(), this);
    ui->mpGallery->setScene(mpGallery);
    mpFullScene = new FullGraphicsScene(this);
    ui->mpView->setScene(mpFullScene);
    ui->mpView->scene()->setSceneRect(0, 0, ui->mpView->width(), ui->mpView->height());

    connect(mpGallery, SIGNAL(active(int)), this, SLOT(setFullImage(int)));
    connect(mpGallery, SIGNAL(needAdd()), this, SLOT(addImage()));
    connect(mpFullScene, SIGNAL(remove(int)), this, SLOT(remove(int)));
    connect(mpFullScene, SIGNAL(back()), this, SLOT(backImage()));
    connect(mpFullScene, SIGNAL(next()), this, SLOT(nextImage()));

}

PhotoAlbumWidget::~PhotoAlbumWidget()
{
    delete ui;
    //delete mpGallery;
    //delete mpFullScene;
}

bool PhotoAlbumWidget::event(QEvent * apEvent)
{
    if ((apEvent->type() == QEvent::Resize) ||
            (apEvent->type() == QEvent::LayoutRequest))
    {
        float vKoff = 0.27;
        ui->mpBack->setMaximumHeight(height() * vKoff);
        ui->mpBack->setMinimumHeight(height() * vKoff);

        ui->mpNext->setMaximumHeight(height() * vKoff);
        ui->mpNext->setMinimumHeight(height() * vKoff);

        ui->mpGallery->setMaximumHeight(height() * vKoff);
        ui->mpGallery->setMinimumHeight(height() * vKoff);
        if (ui->mpGallery->scene())
        {
            ((GalleryGrapicsScene*)ui->mpGallery->scene())->resize(0,0, ui->mpGallery->width()-10, ui->mpGallery->height()*0.8);
        }
        if (ui->mpView->scene())
        {
            ui->mpView->scene()->setSceneRect(0, 0, ui->mpView->width()-10, ui->mpView->height()-10);
        }
    }
    return QWidget::event(apEvent);
}

void PhotoAlbumWidget::addImage()
{
    QFileDialog vDialog(this, TRANSLATE("Добавить фото"));
    if (vDialog.exec() == QDialog::Accepted)
    {
        /*QString vNameFile = QFileDialog::getOpenFileName();
        if (vNameFile.length())
        {
            //mpGallery->addPhoto(vNameFile);
        }*/
        foreach (QString vNameFile, vDialog.selectedFiles()) {
            mpGallery->addPhoto(vNameFile);
        }
    }
}

void PhotoAlbumWidget::on_mpBack_clicked()
{
    ((GalleryGrapicsScene*)ui->mpGallery->scene())->backStep();
}

void PhotoAlbumWidget::on_mpNext_clicked()
{
    ((GalleryGrapicsScene*)ui->mpGallery->scene())->nextStep();
}

void PhotoAlbumWidget::setFullImage(int aId)
{
    ((FullGraphicsScene*)ui->mpView->scene())->setId(aId);
}

void PhotoAlbumWidget::remove(int aId)
{
    ((GalleryGrapicsScene*)ui->mpGallery->scene())->remove(aId);
}

void PhotoAlbumWidget::nextImage()
{
    ((GalleryGrapicsScene*)ui->mpGallery->scene())->next();
}

void PhotoAlbumWidget::backImage()
{
    ((GalleryGrapicsScene*)ui->mpGallery->scene())->back();
}
