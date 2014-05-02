#include "newphotographicsitem.h"
#include "language.h"
#include "gallerygrapicsscene.h"

#include <QApplication>
#include <QGraphicsScene>


NewPhotoGraphicsItem::NewPhotoGraphicsItem(int aIdObject, int aParts, QGraphicsItem *parent) :
    GeneralGraphicsItem(aParts, parent)
  ,mIdObject(aIdObject)
{
}

void NewPhotoGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
           QWidget *widget)
{
    QRectF vRect = boundingRect();

    QLinearGradient vGradient(vRect.topLeft(), vRect.bottomRight());
    vGradient.setColorAt(0.4, Qt::gray);
    vGradient.setColorAt(0.6, Qt::blue);
    painter->fillRect(vRect, vGradient);
    QPen vPen = painter->pen();
    vPen.setWidth(vRect.height() / 20);
    vPen.setColor(painter->background().color());
    painter->setPen(vPen);
    painter->drawRect(vRect);

    vPen.setWidth(vRect.height() / 20);
    painter->setPen(vPen);
    painter->drawRoundRect(vRect);

    GeneralGraphicsItem::paint(painter, option, widget);

    QString vText = TRANSLATE("Добавить\nфото");
    QFont vFont = painter->font();
    vFont.setPixelSize(
                boundingRect().width() /
                (vText.length() / 2)
                );
    painter->setFont(vFont);
    vRect.setRect(vRect.left(),
                  vRect.top(),
                  vRect.width(),
                  vRect.height());
    painter->drawText(vRect, Qt::AlignCenter |Qt::AlignHCenter,vText);
}

void	NewPhotoGraphicsItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    Q_UNUSED(event);
    ((GalleryGrapicsScene*)scene())->newItem();
}
