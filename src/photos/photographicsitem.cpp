#include "photographicsitem.h"
#include "globalsbase.h"
#include "language.h"
#include "gallerygrapicsscene.h"

#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include <QGraphicsView>

#include <QDebug>

PhotoGraphicsItem::PhotoGraphicsItem(int aId, int aParts, QGraphicsItem *parent) :
    GeneralGraphicsItem(aParts, parent)
  , mDeleteImage(":/image/resource/delete.png")
  , isHover(false)
{
    ResponseRecordType vRecord = execQuery(QString("SELECT file, width, height FROM photos WHERE id = %1").arg(aId))[0];
    mImage.loadFromData(QByteArray::fromBase64(vRecord["file"].toByteArray()), "JPG");
    setZValue(-1);
    mId = aId;
    setFlag(QGraphicsItem::ItemIsSelectable);
}

QRectF PhotoGraphicsItem::deleteRect()
{
    QRectF vRect = imageRect();
    qreal vKoff = 0.15;
    qreal vSizeX = vRect.width();
    qreal vSizeY = vRect.height();
    qreal vSize = vSizeX < vSizeY ? vSizeX : vSizeY;
    QRectF vRectDelete = QRectF(
                vRect.x() + vSizeX * (1 - vKoff)
                , vRect.y() /*+ vSize * vKoff*/
                , vSize*vKoff
                , vSize*vKoff);
    return vRectDelete;
}

QRectF PhotoGraphicsItem::imageRect()
{
    QRectF vRect = boundingRect();
    vRect.adjust(vRect.width() * 0.05, vRect.height() * 0.05, 0, 0);
    qreal vMW = mImage.width();
    qreal vMH = mImage.height();
    qreal vKoffX =  vRect.width() / vMW;
    qreal vKoffY =  vRect.height() / vMH;
    qreal vSize = vKoffX < vKoffY ? vKoffX : vKoffY;
    QRectF vNewRect = QRectF(vRect.x(), vRect.y(), vMW * vSize, vMH * vSize);
    return QRectF(vNewRect.x() + (vRect.width() - vRect.x() - vNewRect.width()) / 2
                               , vNewRect.y() + (vRect.height() - vRect.y() - vNewRect.height()) / 2
                               , vNewRect.width()
                               , vNewRect.height());
}

void PhotoGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
           QWidget *widget)
{
    if (isSelected())
    {
        QBrush vOld = painter->brush();
        QBrush vBackground = vOld;
        vBackground.setColor(Qt::gray);
        vBackground.setStyle(Qt::SolidPattern);
        painter->fillRect(boundingRect(), vBackground);
        painter->setBrush(vOld);
    }
    QRectF vRectImage = imageRect();
    painter->drawImage(vRectImage, mImage);
    if (isHover)
    {
        QRectF vRectDelete = deleteRect();
        painter->drawImage(vRectDelete, mDeleteImage.scaled(vRectDelete.width(), vRectDelete.height()));
    }
    GeneralGraphicsItem::paint(painter, option, widget);
}


void	PhotoGraphicsItem::hoverEnterEvent ( QGraphicsSceneHoverEvent * event )
{
    Q_UNUSED(event);
    isHover = true;
    update(deleteRect());
}

void	PhotoGraphicsItem::hoverLeaveEvent ( QGraphicsSceneHoverEvent * event )
{
    Q_UNUSED(event);
    isHover = false;
    update(deleteRect());
}

void	PhotoGraphicsItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    if (deleteRect().contains(event->pos()))
    {
        if (scene())
        {
            if (scene()->views().count())
            {
                int vResult = QMessageBox::question(scene()->views().at(0)->parentWidget()
                                      , TRANSLATE("Удалить фото")
                                      , TRANSLATE("Вы действительно хотите удалить фотографию?")
                                                    , QMessageBox::Ok
                                                    ,QMessageBox::Cancel);
                if (vResult == QMessageBox::Ok)
                {
                    ((GalleryGrapicsScene*)scene())->remove(id());
                }
            }
        }

    }
    else
    {
        setActive();
    }
}

void  PhotoGraphicsItem::setActive()
{
    scene()->clearSelection();
    setSelected(true);
    emit ((GalleryGrapicsScene*)scene())->active(mId);
}

int PhotoGraphicsItem::id() const
{
    return mId;
}

void PhotoGraphicsItem::remove()
{
    execQuery(QString("DELETE FROM photos WHERE id = %1").arg(id()));
}
