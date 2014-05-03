#ifndef PHOTOALBUMWIDGET_H
#define PHOTOALBUMWIDGET_H

#include <QWidget>

#include "gallerygrapicsscene.h"
#include "fullgraphicsscene.h"

namespace Ui {
class PhotoAlbumWidget;
}

class PhotoAlbumWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PhotoAlbumWidget(int aIdObject, QWidget *parent = 0);
    ~PhotoAlbumWidget();
    void setEnabledChangePhoto(bool);
    bool isEnabled() const;
protected:
    bool event(QEvent *);
    
private slots:
    void on_mpBack_clicked();

    void on_mpNext_clicked();
    void setFullImage(int aId);
    void remove(int aId);

    void nextImage();
    void backImage();

    void addImage();
private:
    Ui::PhotoAlbumWidget *ui;
    GalleryGrapicsScene* mpGallery;
    FullGraphicsScene* mpFullScene;
    bool mEnabled;
};

#endif // PHOTOALBUMWIDGET_H
