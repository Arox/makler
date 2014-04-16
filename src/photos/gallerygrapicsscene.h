#ifndef GALLERYGRAPICSSCENE_H
#define GALLERYGRAPICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItemAnimation>
#include <QTimeLine>

#include "newphotographicsitem.h"
#include "photographicsitem.h"

class GalleryGrapicsScene : public QGraphicsScene
{
    Q_OBJECT

    NewPhotoGraphicsItem mNewPhoto;
    static const int mCount;
    int mCurrent;
    QList<PhotoGraphicsItem*> mPhotos;
    QVector <QGraphicsItemAnimation*> mAnimationsNext;
    QVector <QGraphicsItemAnimation*> mAnimationsBack;
    QTimeLine mTimerNext;
    QTimeLine mTimerBack;
    int mCurrentFocus;
    int mIdObject;

    enum TypeStep
    {
        NEXT = 1,
        BACK
    };
    QList<TypeStep> mSteps;
public:
    friend PhotoGraphicsItem;
    explicit GalleryGrapicsScene(int aIdObject, QObject *parent = 0);
    GalleryGrapicsScene(int aIdObject, int x, int y, int width, int height, QObject *parent = 0);
    ~GalleryGrapicsScene();
    void resize(int x, int y, int width, int height);
    void newItem(int aId);
    void newItem();
    void scrollTo(int aIndex);

    bool nextStep();
    bool canNextStep();

    bool backStep();
    bool canBackStep();

    void remove(int aId);

    void next();
    void back();

    void showPhotos(int aStart, int aEnd);
    void showPhoto(int aIndex);
    int beginIndexShow();
    int endIndexShow();
signals:
    void active(int aId);
    void needAdd();
public slots:
    void addPhoto(QString aName);
private slots:
    void endNext();
    void endBack();
    void backChanged(qreal aValue);
protected:
    int findPhotoItem(int aId);
    QPointF pos(int aIndex);
private:
    void inizialisation();
    QPointF _pos(int aIndex);
    void step();
};

#endif // GALLERYGRAPICSSCENE_H
