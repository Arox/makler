#ifndef PHOTOGRAPHICSITEM_H
#define PHOTOGRAPHICSITEM_H

#include "generalgraphicsitem.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QImage>

class PhotoGraphicsItem : public GeneralGraphicsItem
{
    QImage mImage;
    QImage mDeleteImage;
    bool isHover;
    int mId;
public:
    explicit PhotoGraphicsItem(int aId, int aParts, QGraphicsItem *parent = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    int id() const;
    void setActive();
    void remove();
protected:
    virtual void	hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    virtual void	hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void	mousePressEvent ( QGraphicsSceneMouseEvent * event );
    QRectF deleteRect();
    QRectF imageRect();
};

#endif // PHOTOGRAPHICSITEM_H
