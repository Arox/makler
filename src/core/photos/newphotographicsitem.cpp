#include "newphotographicsitem.h"
#include "language.h"
#include "gallerygrapicsscene.h"

#include <QApplication>
#include <QGraphicsScene>


NewPhotoGraphicsItem::NewPhotoGraphicsItem(int aIdObject, int aParts, QGraphicsItem *parent) :
    GeneralGraphicsItem(aParts, parent)
  ,mIdObject(aIdObject)
  ,mIcon(":/photo/add.png")
{
}

QRectF NewPhotoGraphicsItem::boundingRect() const
{
    if (isEnabled())
    {
        return GeneralGraphicsItem::boundingRect();
    }
    else
    {
        return QRectF(0,0,0,0);
    }
}

void NewPhotoGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
           QWidget *widget)
{
    QRectF vRect = boundingRect();

    /*QBrush vOld = painter->brush();
    QBrush vBackground;
    vBackground.setColor(Qt::gray);
    vBackground.setStyle(Qt::SolidPattern);
    painter->fillRect(vRect, vBackground);
    painter->setBrush(vOld);*/

    if (!mIcon.isNull())
    {
        painter->drawImage(vRect.adjusted(10, 10, -10, -10), mIcon);
    }
    QPen vPen = painter->pen();
    vPen.setWidth(vRect.height() / 20);
    vPen.setColor(painter->background().color());
    painter->setPen(vPen);
    painter->drawRect(vRect);

    vPen.setWidth(vRect.height() / 20);
    painter->setPen(vPen);
    painter->drawRoundRect(vRect);

    GeneralGraphicsItem::paint(painter, option, widget);

    QString vText1 = TRANSLATE("Добавить");
    QString vText2 = TRANSLATE("Фото");

    QFont vFont = painter->font();
    if (isHover())
    {
        vFont.setPixelSize(
                    1.7 * boundingRect().width() / vText1.length()
                    );
        vPen.setColor(Qt::black);
    }
    else
    {
        vFont.setPixelSize(
                    1.5 * boundingRect().width() / vText1.length()
                    );
        vPen.setColor(Qt::black);
    }
    painter->setFont(vFont);
    painter->setPen(vPen);
    QRectF vNewRect(
                  vRect.left(),
                  vRect.top(),
                  vRect.width(),
                  vRect.height() / 4);
    painter->drawText(vNewRect, Qt::AlignCenter |Qt::AlignHCenter,vText1);

    vNewRect.setRect(vRect.left(),
                  3  * vRect.height() / 4 + vRect.top(),
                  vRect.width(),
                  vRect.height() / 4);
    painter->drawText(vNewRect, Qt::AlignCenter |Qt::AlignHCenter,vText2);
}

void	NewPhotoGraphicsItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    Q_UNUSED(event);
    if (isEnabled())
    {
        ((GalleryGrapicsScene*)scene())->newItem();
    }
}
