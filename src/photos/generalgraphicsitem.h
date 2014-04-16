#ifndef GENERALGRAPHICSITEM_H
#define GENERALGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

class GeneralGraphicsItem :public QGraphicsItem
{
    int mWidth;
    int mParts;
public:
    explicit GeneralGraphicsItem(int aParts, QGraphicsItem *parent = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    QRectF boundingRect() const;
protected:
    virtual void	hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    virtual void	hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void	hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void	mousePressEvent ( QGraphicsSceneMouseEvent * event );

};

#endif // GENERALGRAPHICSITEM_H
