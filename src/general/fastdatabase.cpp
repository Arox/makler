#include "fastdatabase.h"

FastDatabaseRecord::FastDatabaseRecord(QSqlRecord aResult, QObject *parent):
    QObject(parent),
    mResult(aResult)
{
}

FastDatabaseRecord::FastDatabaseRecord(const FastDatabaseRecord& aObject):
    QObject(aObject.parent()),
    mResult(aObject.toRecord())
{
}

FastDatabaseRecord FastDatabaseRecord::operator=(const FastDatabaseRecord& aObject)
{
    mResult = aObject.toRecord();
    return *this;
}

QVariant FastDatabaseRecord::operator[](QString aName) const
{
    return mResult.value(mResult.indexOf(aName));
}

void FastDatabaseRecord::setData(QString aName, QVariant aValue)
{
    mResult.setValue(aName, aValue);
}

QSqlRecord FastDatabaseRecord::toRecord() const
{
    return mResult;
}



FastDatabaseResult::FastDatabaseResult(QSqlQuery *apResult, QObject *parent) :
    QObject(parent),
    mpResults(apResult),
    mCount(0),
    mBegin(this),
    mEnd(this),
    mRow(0)
{
    while(mpResults->next())
    {
        ++mCount;
    }
    mpResults->first();
}

FastDatabaseResult::FastDatabaseResult(QObject *parent):
    QObject(parent),
    mpResults(0),
    mCount(0),
    mBegin(this),
    mEnd(this),
    mRow(0)
{

}

FastDatabaseResult::FastDatabaseResult(const FastDatabaseResult& aObject):
    QObject(aObject.parent()),
    mpResults(0),
    mCount(0),
    mBegin(this),
    mEnd(this),
    mRow(0)

{
    mpResults = aObject.data();
    mpResults->first();
    mCount = aObject.count();
    mEnd.setRow(mCount);
}

FastDatabaseResult FastDatabaseResult::operator=(const FastDatabaseResult& aObject)
{
    mpResults = aObject.data();
    mCount = aObject.count();
    mEnd.setRow(mCount);
    return *this;
}

QSqlQuery* FastDatabaseResult::data() const
{
    return mpResults;
}

FastDatabaseRecord FastDatabaseResult::operator[](int aRow) const
{
    return this->at(aRow);
}

int FastDatabaseResult::count() const
{
    return mCount;
}

FastDatabaseRecord FastDatabaseResult::at(int aRow) const
{
    /*if (mRow > aRow)
    {
        mRow = 0;
        mpResults->first();
    }*/

    int i = 0;
    while (i < aRow)
    {
        if (!mpResults->next())
        {
            return FastDatabaseRecord();
        }
        ++i;
    }
    FastDatabaseRecord vResult(mpResults->record());
    mpResults->first();
    return vResult;
}

FastDatabaseResult::const_iterator::const_iterator(FastDatabaseResult *apResult, int aRow):
    mRow(aRow),
    mpResult(apResult)
{

}

FastDatabaseResult::const_iterator::const_iterator(const const_iterator& aObject):
    mRow(aObject.row()),
    mpResult(aObject.data())
{

}

FastDatabaseResult* FastDatabaseResult::const_iterator::data() const
{
    return mpResult;
}

const FastDatabaseRecord& FastDatabaseResult::const_iterator::operator*()
{
    mCurrent = mpResult->at(mRow);
    return mCurrent;
}

int FastDatabaseResult::const_iterator::row() const
{
    return mRow;
}

bool FastDatabaseResult::const_iterator::operator!=( const FastDatabaseResult::const_iterator& iterator) const
{
    return this->row() != iterator.row();
}

FastDatabaseResult::const_iterator& FastDatabaseResult::const_iterator::operator++()
{
   ++mRow;
   return *this;
}

void FastDatabaseResult::const_iterator::setRow(int aRow)
{
    mRow = aRow;
}

FastDatabaseResult::const_iterator FastDatabaseResult::begin() const
{
    return mBegin;
}

FastDatabaseResult::const_iterator FastDatabaseResult::end() const
{
    return mEnd;
}

