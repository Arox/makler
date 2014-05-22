#include "fullgraphicsitem.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include "globalsbase.h"
#include "fullgraphicsscene.h"
#include <QDebug>

FullGraphicsItem::FullGraphicsItem(int aId, QGraphicsItem *parent):
    QGraphicsItem(parent)
  , mImageDelete(":/image/resource/delete.png")
  , mImageBack(":/image/resource/back.png")
  , mImageNext(":/image/resource/next.png")
  , mId(aId)
  , mNext(false)
  , mBack(false)
  , mDelete(false)
{
    ResponseRecordType vRecord = execQuery(QString("SELECT file, width, height FROM photos WHERE id = %1").arg(aId))[0];
    mImage.loadFromData(QByteArray::fromBase64(vRecord["file"].toByteArray()), "JPG");
    setAcceptHoverEvents(true);
}

void FullGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QRectF vRectDraw = imageRect();
    painter->drawImage(vRectDraw, mImage);
    if (mDelete)
    {
        QRectF vDeleteRect = deleteRect();
        painter->drawImage(vDeleteRect, mImageDelete);
    }
    qreal vSizeBackNext = 0.1;
    if (mBack)
    {
        QRectF vLeft = leftRect();

        QRectF vBackRect = QRectF(vLeft.x() + vLeft.width() * vSizeBackNext
                                  , vLeft.y() + vLeft.height() / 2 - vLeft.height() * vSizeBackNext / 2
                                  , vLeft.width() * vSizeBackNext
                                  , vLeft.height() * vSizeBackNext);
        painter->drawImage(vBackRect, mImageBack);
    }
    if (mNext)
    {
        QRectF vRight = rightRect();
        QRectF vNextRect = QRectF(vRight.x() + vRight.width() - vRight.width() * vSizeBackNext * 2
                                  , vRight.y() + vRight.height() / 2 - vRight.height() * vSizeBackNext / 2
                                  , vRight.width() * vSizeBackNext
                                  , vRight.height() * vSizeBackNext);
        painter->drawImage(vNextRect, mImageNext);
    }
}

QRectF FullGraphicsItem::boundingRect() const
{
    if (scene())
    {
        QRectF vRect = scene()->sceneRect();
        return QRectF(0, 0, vRect.width(), vRect.height());
    }
    return QRectF();
}

QRectF FullGraphicsItem::imageRect()
{
    QRectF vRect = boundingRect();
    qreal vMW = mImage.width();
    qreal vMH = mImage.height();
    qreal vKoffX =  vRect.width() / vMW;
    qreal vKoffY =  vRect.height() / vMH;
    qreal vSize = vKoffX < vKoffY ? vKoffX : vKoffY;
    QRectF vNewRect = QRectF(vRect.x(), vRect.y(), vMW * vSize, vMH * vSize);
    return QRectF(vRect.x() + (vRect.width() - vRect.x() - vNewRect.width()) / 2
                              , vRect.y() + (vRect.height() - vRect.y() - vNewRect.height()) / 2
                              , vNewRect.width()
                              , vNewRect.height());

}

QRectF FullGraphicsItem::deleteRect()
{
    QRectF vRectDraw = imageRect();
    qreal vSizeDelete = vRectDraw.width() < vRectDraw.height() ? vRectDraw.width() : vRectDraw.height();
    return QRectF(vRectDraw.x() + vRectDraw.width() * 0.9
                                , vRectDraw.y() + vRectDraw.height() * 0.05
                                , vSizeDelete * 0.05
                                , vSizeDelete * 0.05);
}

QRectF FullGraphicsItem::leftRect()
{
    QRectF vLeft = boundingRect();
    return QRectF(0, 0, vLeft.width()-vLeft.width() / 2, vLeft.height());
}

QRectF FullGraphicsItem::rightRect()
{
    QRectF vLeft = boundingRect();
    return vLeft.adjusted(vLeft.width() / 2, 0, 0, 0);
}

void	FullGraphicsItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{

    if (leftRect().contains(event->pos()))
    {
        emit ((FullGraphicsScene*)scene())->back();
    }
    else
    {
        if (rightRect().contains(event->pos()))
        {
            emit ((FullGraphicsScene*)scene())->next();
        }
    }
}

void	FullGraphicsItem::hoverEnterEvent ( QGraphicsSceneHoverEvent * event )
{
    Q_UNUSED(event);
    mDelete = true;
}

void	FullGraphicsItem::hoverLeaveEvent ( QGraphicsSceneHoverEvent * event )
{
    Q_UNUSED(event);
    mDelete = false;
    mNext = false;
    mBack = false;
}

void	FullGraphicsItem::hoverMoveEvent ( QGraphicsSceneHoverEvent * event )
{
    QRectF vLeft = leftRect();
    if (vLeft.contains(event->pos()))
    {
        if (!mBack)
        {
            mBack = true;
            mNext = false;
            update(boundingRect());
        }
    }
    else
    {
        QRectF vRight = rightRect();
        if (vRight.contains(event->pos()))
        {
            if (!mNext)
            {
                mNext = true;
                mBack = false;
                update(boundingRect());
            }

        }
    }
}

int FullGraphicsItem::id()
{
    return mId;
}
