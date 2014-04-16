#ifndef NEWPHOTOGRAPHICSITEM_H
#define NEWPHOTOGRAPHICSITEM_H

#include "generalgraphicsitem.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

class NewPhotoGraphicsItem : public GeneralGraphicsItem
{
    int mIdObject;
public:
    explicit NewPhotoGraphicsItem(int aIdObject, int aParts, QGraphicsItem *parent = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
protected:
    virtual void	mousePressEvent ( QGraphicsSceneMouseEvent * event );
};

#endif // NEWPHOTOGRAPHICSITEM_H
