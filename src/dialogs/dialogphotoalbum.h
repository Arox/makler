#ifndef DIALOGPHOTOALBUM_H
#define DIALOGPHOTOALBUM_H

#include <QDialog>

#include "photoalbumwidget.h"

namespace Ui {
class DialogPhotoAlbum;
}

class DialogPhotoAlbum : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogPhotoAlbum(int aIdObject, QWidget *parent = 0);
    ~DialogPhotoAlbum();
    void setChangePhotoEnabled(bool aEnabled);
private:
    Ui::DialogPhotoAlbum *ui;
    PhotoAlbumWidget mPhotoAlbum;
};

#endif // DIALOGPHOTOALBUM_H
