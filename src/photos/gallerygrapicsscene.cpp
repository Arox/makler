#include "gallerygrapicsscene.h"
#include "globalsbase.h"

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QBuffer>
#include <QTime>
#include <QFileDialog>

const int GalleryGrapicsScene::mCount = 5;

void GalleryGrapicsScene::inizialisation()
{
    mCurrentFocus = -1;
    addItem(&mNewPhoto);
    mNewPhoto.setPos(sceneRect().width() - mNewPhoto.boundingRect().width(), 0);
    mTimerNext.setFrameRange(0, 100);
    mTimerBack.setFrameRange(0, 100);


    for (int i = 0; i < mCount; i++)
    {
        QGraphicsItemAnimation* vpAnimation = new QGraphicsItemAnimation();
        vpAnimation->setTimeLine(&mTimerNext);
        mAnimationsNext[i] = vpAnimation;

    }

    for (int i = 0; i < mCount; i++)
    {
        QGraphicsItemAnimation* vpAnimation = new QGraphicsItemAnimation();
        vpAnimation->setTimeLine(&mTimerBack);
        mAnimationsBack[i] = vpAnimation;

    }

    connect(&mTimerNext, SIGNAL(finished()), this, SLOT(endNext()));
    connect(&mTimerBack, SIGNAL(finished()), this, SLOT(endBack()));
    connect(&mTimerBack, SIGNAL(valueChanged(qreal)), this, SLOT(backChanged(qreal)));

    ResponseType vResponse = execQuery(QString("SELECT id FROM photos WHERE object_fk = %1")
                                       .arg(mIdObject));
    foreach (ResponseRecordType vRecord, vResponse)
    {
        PhotoGraphicsItem* vpPhoto = new PhotoGraphicsItem(vRecord["id"].toInt(), mCount);
        mPhotos.append(vpPhoto);
    }
    int vCount = mCount - 1;
    if (vCount < mPhotos.count())
    {
        showPhotos(mPhotos.count() - vCount, mPhotos.count() - 1);
    }
    else
    {
        showPhotos(0, mPhotos.count() ? mPhotos.count() - 1 : -1);
    }
}

GalleryGrapicsScene::GalleryGrapicsScene(int aIdObject, QObject *parent) :
    QGraphicsScene(parent)
  , mNewPhoto(aIdObject, mCount)
  , mCurrent(-1)
  , mAnimationsNext(mCount)
  , mAnimationsBack(mCount)
  , mTimerNext(1000)
  , mTimerBack(1000)
  ,mIdObject(aIdObject)
{
    inizialisation();
}

GalleryGrapicsScene::GalleryGrapicsScene(int aIdObject, int x, int y, int width, int height, QObject *parent):
    QGraphicsScene(x, y, width, height, parent)
  , mNewPhoto(aIdObject, mCount)
  , mCurrent(-1)
  , mAnimationsNext(mCount)
  , mAnimationsBack(mCount)
  , mTimerNext(1000)
  , mTimerBack(1000)
  ,mIdObject(aIdObject)
{
    inizialisation();
}

GalleryGrapicsScene::~GalleryGrapicsScene()
{
    foreach (PhotoGraphicsItem* vpPhoto, mPhotos)
    {
        delete vpPhoto;
    }
    mPhotos.clear();
}

void GalleryGrapicsScene::resize(int x, int y, int width, int height)
{
    setSceneRect(x, y, width, height);
    mNewPhoto.setPos(sceneRect().width() - mNewPhoto.boundingRect().width(), 0);
    for (int i = 0; i < mCount - 1; ++i)
    {
        int vIndex = mCurrent - mCount + 2 + i;
        if (mCurrent < (mCount - 1)) vIndex = i;
        if (vIndex >= mPhotos.count()) break;
        if (vIndex < 0) continue;
         mPhotos.at(vIndex)->setPos(pos(i));
    }
}

void GalleryGrapicsScene::showPhotos(int aStart, int aEnd)
{
    mSteps.clear();
    mTimerBack.stop();
    mTimerNext.stop();
    int vCount = aEnd - aStart+1;
    for (int i = 0; i < vCount; ++i)
    {
        mPhotos.at(aStart + i)->setPos(pos(i));
        addItem(mPhotos.at(aStart + i));
    }
    mCurrent = aEnd;
}

void GalleryGrapicsScene::newItem(int aId)
{
    PhotoGraphicsItem* vpPhoto = new PhotoGraphicsItem(aId, mCount);
    mPhotos.append(vpPhoto);
    if (mCurrent < (mCount - 2))
    {
        mCurrent += 1;
    }
    scrollTo(mPhotos.count() - 1);
}

void GalleryGrapicsScene::addPhoto(QString aName)
{
    QString vNameFile = aName;
    if (vNameFile.length())
    {
        qsrand(QTime::currentTime().msec());
        int vRand = qrand() % 100000 + 1;
        execQuery(QString("INSERT INTO photos (object_fk, rand) VALUES (%1, %2)").arg(mIdObject).arg(vRand));
        int vId = execQuery(QString("SELECT id FROM photos WHERE object_fk = %1 AND rand = %2")
                            .arg(mIdObject)
                            .arg(vRand))[0]["id"].toInt();

        QImage vFile(vNameFile);
        QByteArray vArray;
        QBuffer vBuffer(&vArray);
        vBuffer.open(QBuffer::WriteOnly);
        vFile.save(&vBuffer, "JPG");

        QSqlQuery vQuery(QSqlDatabase::database("local"));
        vQuery.prepare("UPDATE photos SET file = :photo, rand = :rand, height = :height, width = :width WHERE id = :id");
        vQuery.bindValue(":id", vId);
        vQuery.bindValue(":photo", vArray.toBase64());
        vQuery.bindValue(":rand", 0);
        vQuery.bindValue(":height", vFile.height());
        vQuery.bindValue(":width", vFile.width());
        vQuery.exec();
        if (vQuery.lastError().isValid())
        {
            qDebug() << vQuery.lastError().text();
        }
        else
        {
            newItem(vId);
        }
        vBuffer.close();
    }
}

void GalleryGrapicsScene::newItem()
{
    emit needAdd();
}

bool GalleryGrapicsScene::canNextStep()
{
    if (mPhotos.count() < (mCount - 1))
        return false;
    if (mCurrent >= (mPhotos.count() - 1))
        return false;
    return true;
}

bool GalleryGrapicsScene::nextStep()
{
    if ((mTimerNext.state() == QTimeLine::Running) || (mTimerBack.state() == QTimeLine::Running))
    {
        mSteps.append(NEXT);
        return false;
    }
    if (!canNextStep())
    {
        endNext();
        return false;
    }
    mPhotos.at(mCurrent + 1)->setPos(_pos(mCount - 1));
    for (int i = 0; i < mCount; ++i)
    {
        mAnimationsNext[i]->setItem(mPhotos.at(mCurrent - mCount + i + 2));
        mAnimationsNext[i]->setTimeLine(&mTimerNext);
        QPointF vPosBegin = _pos(i);
        QPointF vPosEnd = _pos(i - 1);
        qreal vLength = 200;
        qreal vStepX = qAbs(vPosBegin.x() - vPosEnd.x())/vLength;
        qreal vStepY = qAbs(vPosBegin.y() - vPosEnd.y())/vLength;
        for (int j = 0; j < vLength; j++)
        {
            QPointF vPoint = QPointF(vPosBegin.x() - j *vStepX, vPosBegin.y() - j *vStepY);
            mAnimationsNext[i]->setPosAt(j / vLength, vPoint);
        }

    }
    addItem(mPhotos.at(mCurrent + 1));
    mTimerNext.start();
    ++mCurrent;
    return true;
}

bool GalleryGrapicsScene::backStep()
{
    if ((mTimerNext.state() == QTimeLine::Running) || (mTimerBack.state() == QTimeLine::Running))
    {
        mSteps.append(BACK);
        return false;
    }
    if (!canBackStep())
    {
        endBack();
        return false;
    }
    mPhotos.at(mCurrent - mCount + 1)->setPos(_pos(-1));
    addItem(mPhotos.at(mCurrent - mCount + 1));
    for (int i = 0; i < mCount; ++i)
    {
        mAnimationsBack[i]->setItem(mPhotos.at(mCurrent - mCount + i + 1));
        mAnimationsBack[i]->setTimeLine(&mTimerBack);
        QPointF vPosBegin = _pos(i-1);
        QPointF vPosEnd = _pos(i);
        qreal vLength = 200;
        qreal vStepX = qAbs(vPosBegin.x() - vPosEnd.x())/vLength;
        qreal vStepY = qAbs(vPosBegin.y() - vPosEnd.y())/vLength;
        for (int j = 0; j < vLength; j++)
        {
            QPointF vPoint = QPointF(vPosBegin.x() + j *vStepX, vPosBegin.y() - j *vStepY);
            mAnimationsBack[i]->setPosAt(j / vLength, vPoint);
        }

    }
    addItem(mPhotos.at(mCurrent - mCount + 2));
    mTimerBack.start();
    --mCurrent;
    return true;
}

bool GalleryGrapicsScene::canBackStep()
{
    if (mCurrent < (mCount - 1))
        return false;
    return true;
}

void GalleryGrapicsScene::scrollTo(int aIndex)
{
    if (aIndex <= (mCount - 2))
    {
        mPhotos.at(aIndex)->setPos(pos(aIndex));
        addItem(mPhotos.at(aIndex));
    }
    if (aIndex > mCurrent)
    {
        while (mCurrent < aIndex) nextStep();
    }
    else
    {
        if (aIndex < (mCurrent - mCount + 2))
        {
            while (aIndex < (mCurrent - mCount + 2)) backStep();
        }
    }
}

QPointF GalleryGrapicsScene::pos(int aIndex)
{
    return (aIndex < mCount - 1 ? _pos(aIndex) : _pos(aIndex + 1));
}

QPointF GalleryGrapicsScene::_pos(int aIndex)
{
    qreal vW = sceneRect().width() / mCount;
    qreal x = vW* aIndex + vW*0.05;
    qreal y = 0;
    return QPointF(x, y);
}

void GalleryGrapicsScene::step()
{
    if (mSteps.count())
    {
        TypeStep vStep = mSteps.first();
        mSteps.pop_front();
        if (vStep == NEXT)
        {
            nextStep();
        }
        if (vStep == BACK)
        {
            backStep();
        }
    }
}

void GalleryGrapicsScene::endNext()
{
    if ((mCurrent - mCount + 1 > 0))
    {
        removeItem(mPhotos.at(mCurrent - mCount + 1));
    }
    update(sceneRect());
    if (!selectedItems().count())
    {
        if (mCurrent >= 0)
        {
            mPhotos.at(mCurrent - mCount + 2)->setActive();
        }
    }
    step();
}

void GalleryGrapicsScene::endBack()
{
    if (mPhotos.count() > (mCurrent + 1))
    {
        removeItem(mPhotos.at(mCurrent + 1));
    }
    update(sceneRect());
    if (!selectedItems().count())
    {
        if (mCurrent >= 0)
        {
            mPhotos.at(mCurrent)->setActive();
        }
    }

    step();
}

void GalleryGrapicsScene::backChanged(qreal aValue)
{
    Q_UNUSED(aValue);
}

int GalleryGrapicsScene::findPhotoItem(int aId)
{
    int i = 0;
    for (; i < mPhotos.count(); ++i)
    {
        if (mPhotos.at(i)->id() == aId)
        {
            break;
        }
    }
    if (i == mPhotos.count()) i = -1;
    return i;
}

void GalleryGrapicsScene::remove(int aId)
{
    int i = findPhotoItem(aId);
    if (mCurrentFocus == i) mCurrentFocus = -1;
    Q_ASSERT(mPhotos.count() > i);
    removeItem(mPhotos.at(i));
    mPhotos.at(i)->remove();
    mPhotos.removeAt(i);
    if (mCurrent < (mPhotos.count() - 1))
    {
        for (int i = 0; i < mCount - 1; ++i)
        {
            mPhotos[mCurrent - (mCount - 2) + i]->setPos(pos(i));
        }
        addItem(mPhotos[mCurrent]);
    }
    else
    {
        mCurrent -= 1;
        int j = 0;
        for (int i = mCurrent - mCount + 2; i <= mCurrent; ++i)
        {
            if (i < 0) continue;
            if (j == 0) addItem(mPhotos[i]);
            mPhotos[i]->setPos(pos(j));
            ++j;
        }

    }
    update(sceneRect());
}

void GalleryGrapicsScene::next()
{
    if (selectedItems().count())
    {
        PhotoGraphicsItem* vpItem = (PhotoGraphicsItem*)selectedItems().at(0);
        int i = findPhotoItem(vpItem->id());
        if ((i>=0) && (i < (mPhotos.count() - 1)))
        {
            PhotoGraphicsItem* vpNewItem = mPhotos.at(i+1);
            //scrollTo(i+1);
            showPhoto(i + 1);
            vpNewItem->setActive();
        }
    }
}

void GalleryGrapicsScene::back()
{
    if (selectedItems().count())
    {
        PhotoGraphicsItem* vpItem = (PhotoGraphicsItem*)selectedItems().at(0);
        int i = findPhotoItem(vpItem->id());
        if ((i > 0) && (i < mPhotos.count()))
        {
            PhotoGraphicsItem* vpNewItem = mPhotos.at(i-1);
            //scrollTo(i-1);
            showPhoto(i - 1);
            vpNewItem->setActive();
        }
    }
}

void GalleryGrapicsScene::showPhoto(int aIndex)
{
    int vOldStart = beginIndexShow();
    int vOldEnd = endIndexShow();
    if ((aIndex >= vOldStart) && (aIndex <= vOldEnd)) return;
    for (int i = mCurrent; i > 0; --i)
    {
        removeItem(mPhotos.at(i));
    }
    if (aIndex < vOldStart)
    {
        if (aIndex < (mCount - 2))
        {
            showPhotos(0, mCount - 2);
            return;
        }

    }
    showPhotos(aIndex - mCount + 2, aIndex);
    return;
}

int GalleryGrapicsScene::beginIndexShow()
{
    if (mCurrent < 0) return -1;
    if (mCurrent <= (mCount-2))
    {
        return 0;
    }
    return mCurrent - mCount + 2;
}

int GalleryGrapicsScene::endIndexShow()
{
    return mCurrent;
}
