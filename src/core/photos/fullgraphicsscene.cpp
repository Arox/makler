#include "fullgraphicsscene.h"

FullGraphicsScene::FullGraphicsScene(QObject *parent):
    QGraphicsScene(parent)
{
    mItem = 0;
}

FullGraphicsScene::~FullGraphicsScene()
{
    if (mItem)
    {
        removeItem(mItem);
        delete mItem;
        mItem = 0;
    }
}

void FullGraphicsScene::setId(int aId)
{
    if (mItem)
    {
        removeItem(mItem);
        delete mItem;
    }
    mItem = new FullGraphicsItem(aId);
    mItem->setPos(5,0);
    addItem(mItem);
}

void FullGraphicsScene::removeElement()
{
    if (items().count())
    {
        int vId = mItem->id();
        removeItem(mItem);
        delete mItem;
        mItem = 0;
        emit remove(vId);
    }
}
