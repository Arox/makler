#include "generalgraphicsitem.h"
#include "globalsbase.h"

#include <QApplication>
#include <QGraphicsScene>

GeneralGraphicsItem::GeneralGraphicsItem(int aParts, QGraphicsItem *parent) :
    QGraphicsItem(parent), mWidth(0),
    mParts(aParts)
  , mEnabled(true)
{
    setAcceptHoverEvents(true);
}

QRectF GeneralGraphicsItem::boundingRect() const
{
    qreal vW = 0;
    QRectF vRect(0, 0, 0, 0);
    if(scene())
    {
        vW = scene()->width() / mParts;
        vRect = QRectF(0, 0, vW - vW * 0.05, scene()->height());
    }
    return vRect;
}

void GeneralGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
           QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (mWidth)
    {
        QRectF vRect = boundingRect();
        QPen vPen = painter->pen();
        vPen.setWidth(mWidth);
        vPen.setColor(Qt::black);
        painter->setPen(vPen);
        painter->drawRoundRect(vRect);
    }
}


void	GeneralGraphicsItem::hoverEnterEvent ( QGraphicsSceneHoverEvent * event )
{
    Q_UNUSED(event);
    mWidth = 4;
    update(boundingRect());
}

void	GeneralGraphicsItem::hoverLeaveEvent ( QGraphicsSceneHoverEvent * event )
{
    Q_UNUSED(event);
    mWidth = 0;
    update(boundingRect());
}

void	GeneralGraphicsItem::hoverMoveEvent ( QGraphicsSceneHoverEvent * event )
{
    Q_UNUSED(event);
}

void	GeneralGraphicsItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    Q_UNUSED(event);
}



void GeneralGraphicsItem::setEnabledChangePhoto(bool aEnable)
{
    mEnabled = aEnable;
}

bool GeneralGraphicsItem::isEnabled() const
{
    return mEnabled;
}

