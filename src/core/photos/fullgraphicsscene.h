#ifndef FULLGRAPHICSSCENE_H
#define FULLGRAPHICSSCENE_H

#include <QGraphicsScene>

#include "fullgraphicsitem.h"

class FullGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
    FullGraphicsItem* mItem;
public:
    friend FullGraphicsItem;
    FullGraphicsScene(QObject *parent = 0);
    ~FullGraphicsScene();
    void setId(int aId);
    void removeElement();
signals:
    void remove(int aId);
    void next();
    void back();
};

#endif // FULLGRAPHICSSCENE_H
