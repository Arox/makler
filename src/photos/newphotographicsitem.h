#ifndef NEWPHOTOGRAPHICSITEM_H
#define NEWPHOTOGRAPHICSITEM_H

#include "generalgraphicsitem.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QImage>

class NewPhotoGraphicsItem : public GeneralGraphicsItem
{
    int mIdObject;
    QImage mIcon;
public:
    explicit NewPhotoGraphicsItem(int aIdObject, int aParts, QGraphicsItem *parent = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    QRectF boundingRect() const;
protected:
    virtual void	mousePressEvent ( QGraphicsSceneMouseEvent * event );
};

#endif // NEWPHOTOGRAPHICSITEM_H
