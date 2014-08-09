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
    reset();

    if (isChangeEnabled())
    {
        addItem(&mNewPhoto);
        mNewPhoto.setPos(sceneRect().width() - mNewPhoto.boundingRect().width(), 0);
    }
    mTimerNext.setFrameRange(0, 100);
    mTimerBack.setFrameRange(0, 100);


    for (int i = 0; i < countPhotos() + 1; i++)
    {
        QGraphicsItemAnimation* vpAnimation = new QGraphicsItemAnimation();
        vpAnimation->setTimeLine(&mTimerNext);
        mAnimationsNext.insert(i, vpAnimation);

    }

    for (int i = 0; i < countPhotos() + 1; i++)
    {
        QGraphicsItemAnimation* vpAnimation = new QGraphicsItemAnimation();
        vpAnimation->setTimeLine(&mTimerBack);
        mAnimationsBack.insert(i, vpAnimation);
    }

    connect(&mTimerNext, SIGNAL(finished()), this, SLOT(endNext()));
    connect(&mTimerBack, SIGNAL(finished()), this, SLOT(endBack()));
    connect(&mTimerBack, SIGNAL(valueChanged(qreal)), this, SLOT(backChanged(qreal)));

    ResponseType vResponse = execQuery(QString("SELECT id FROM photos WHERE object_fk = %1")
                                       .arg(mIdObject));
    foreach (ResponseRecordType vRecord, vResponse)
    {
        PhotoGraphicsItem* vpPhoto = new PhotoGraphicsItem(vRecord["id"].toInt(), count());
        mPhotos.append(vpPhoto);
    }
    if (countPhotos() < mPhotos.count())
    {
        showPhotos(mPhotos.count() - countPhotos(), mPhotos.count() - 1);
    }
    else
    {
        showPhotos(0, mPhotos.count() ? mPhotos.count() - 1 : -1);
    }
}

GalleryGrapicsScene::GalleryGrapicsScene(int aIdObject, QObject *parent) :
    QGraphicsScene(parent)
  , mNewPhoto(aIdObject, count())
  , mCurrent(-1)
  , mAnimationsNext(count())
  , mAnimationsBack(count())
  , mTimerNext(1000)
  , mTimerBack(1000)
  ,mIdObject(aIdObject)
  ,mEnabled(true)
{
    inizialisation();
}

GalleryGrapicsScene::GalleryGrapicsScene(int aIdObject, int x, int y, int width, int height, QObject *parent):
    QGraphicsScene(x, y, width, height, parent)
  , mNewPhoto(aIdObject, count())
  , mCurrent(-1)
  , mAnimationsNext(count())
  , mAnimationsBack(count())
  , mTimerNext(1000)
  , mTimerBack(1000)
  ,mIdObject(aIdObject)
  ,mEnabled(true)
{
    inizialisation();
}

GalleryGrapicsScene::~GalleryGrapicsScene()
{
    reset();
}

void GalleryGrapicsScene::reset()
{
    foreach (QGraphicsItemAnimation* vpItem, mAnimationsBack) {
        delete vpItem;
    }
    mAnimationsBack.clear();

    foreach (QGraphicsItemAnimation* vpItem, mAnimationsNext) {
        delete vpItem;
    }
    mAnimationsNext.clear();
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
    for (int i = 0; i < countPhotos(); ++i)
    {
        int vIndex = mCurrent - lastIndexPhotos() + i;
        if (mCurrent < countPhotos()) vIndex = i;
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
    PhotoGraphicsItem* vpPhoto = new PhotoGraphicsItem(aId, count());
    vpPhoto->setEnabled(isChangeEnabled());
    mPhotos.append(vpPhoto);
    if (mCurrent < lastIndexPhotos())
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
    if (mPhotos.count() < countPhotos())
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
    mPhotos.at(mCurrent + 1)->setPos(_pos(countPhotos()));
    for (int i = 1; i < countPhotos() + 1; ++i)
    {
        mAnimationsNext[i]->setItem(mPhotos.at(mCurrent - lastIndexPhotos() + i));
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
    removeItem(mPhotos.at(mCurrent - lastIndexPhotos()));
    if (!mPhotos.at(mCurrent + 1)->scene())
    {
        addItem(mPhotos.at(mCurrent + 1));
    }
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
    mPhotos.at(mCurrent - countPhotos())->setPos(_pos(-1));
    if (!mPhotos.at(mCurrent - countPhotos())->scene())
    {
        addItem(mPhotos.at(mCurrent - countPhotos()));
    }
    for (int i = 0; i < countPhotos(); ++i)
    {
        mAnimationsBack[i]->setItem(mPhotos.at(mCurrent - countPhotos() + i));
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
    removeItem(mPhotos.at(mCurrent));
    if (!mPhotos.at(mCurrent - lastIndexPhotos())->scene())
    {
        addItem(mPhotos.at(mCurrent - lastIndexPhotos()));
    }
    mTimerBack.start();
    --mCurrent;
    return true;
}

bool GalleryGrapicsScene::canBackStep()
{
    if (mCurrent < countPhotos())
        return false;
    return true;
}

void GalleryGrapicsScene::scrollTo(int aIndex)
{
    if (aIndex < countPhotos())
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
        while (aIndex < (mCurrent - lastIndexPhotos())) backStep();
    }
}

QPointF GalleryGrapicsScene::pos(int aIndex)
{
    return (aIndex < countPhotos() ? _pos(aIndex) : _pos(aIndex + 1));
}

QPointF GalleryGrapicsScene::_pos(int aIndex)
{
    qreal vW = sceneRect().width() / count();
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
    if ((mCurrent - countPhotos() > 0))
    {
        if (mCurrent < (mPhotos.count() - 1))
        {
            removeItem(mPhotos.at(mCurrent - countPhotos()));
        }
    }
    update(sceneRect());
    if (!selectedItems().count())
    {
        if (mCurrent >= 0)
        {
            mPhotos.at(mCurrent - lastIndexPhotos())->setActive();
        }
    }
    step();
}

void GalleryGrapicsScene::endBack()
{
    if (mPhotos.count() > (mCurrent + 1))
    {
        if ((mCurrent + 1) < countPhotos())
        {
            removeItem(mPhotos.at(mCurrent + 1));
        }
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
        for (int i = 0; i < countPhotos(); ++i)
        {
            mPhotos[mCurrent - lastIndexPhotos() + i]->setPos(pos(i));
        }
        addItem(mPhotos[mCurrent]);
    }
    else
    {
        mCurrent -= 1;
        int j = 0;
        for (int i = mCurrent - lastIndexPhotos(); i <= mCurrent; ++i)
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
        if (aIndex < lastIndexPhotos())
        {
            showPhotos(0, lastIndexPhotos());
            return;
        }

    }
    showPhotos(aIndex - lastIndexPhotos(), aIndex);
    return;
}

int GalleryGrapicsScene::beginIndexShow()
{
    if (mCurrent < 0) return -1;
    if (mCurrent < countPhotos())
    {
        return 0;
    }
    return mCurrent - lastIndexPhotos();
}

int GalleryGrapicsScene::endIndexShow()
{
    return mCurrent;
}

void GalleryGrapicsScene::setEnabledChangePhoto(bool aEnabled)
{
    mEnabled = aEnabled;
    foreach (PhotoGraphicsItem* vpPhoto, mPhotos)
    {
        vpPhoto->setEnabledChangePhoto(isChangeEnabled());
        if (vpPhoto->scene()) removeItem(vpPhoto);
    }
    if (mNewPhoto.scene()) removeItem(&mNewPhoto);
    inizialisation();
    mNewPhoto.setEnabledChangePhoto(isChangeEnabled());
}

bool GalleryGrapicsScene::isChangeEnabled() const
{
    return mEnabled;
}

int GalleryGrapicsScene::countPhotos() const
{
    return isChangeEnabled() ? mCount - 1 : mCount;
}

int GalleryGrapicsScene::lastIndexPhotos() const
{
    return isChangeEnabled() ? mCount - 2 : mCount - 1;
}

int GalleryGrapicsScene::count() const
{
    return mCount;
}
