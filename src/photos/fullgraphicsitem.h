#ifndef FULLGRAPHICSITEM_H
#define FULLGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QImage>

class FullGraphicsItem : public QGraphicsItem
{
    QImage mImage;
    QImage mImageDelete;
    QImage mImageBack;
    QImage mImageNext;
    int mId;

    bool mNext;
    bool mBack;
    bool mDelete;
public:
    explicit FullGraphicsItem(int aId, QGraphicsItem *parent = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    QRectF boundingRect() const;
    int id();
protected:
    QRectF imageRect();
    QRectF deleteRect();
    QRectF leftRect();
    QRectF rightRect();
    virtual void	mousePressEvent ( QGraphicsSceneMouseEvent * event );
    virtual void	hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    virtual void	hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void	hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
};

#endif // FULLGRAPHICSITEM_H
