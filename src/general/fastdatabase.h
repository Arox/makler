#ifndef FASTDATABASE_H
#define FASTDATABASE_H

#include <QObject>
#include <QSqlResult>
#include <QSqlQuery>
#include <QSqlRecord>

class FastDatabaseRecord: public QObject
{
    Q_OBJECT
public:
    explicit FastDatabaseRecord(QSqlRecord aResult = QSqlRecord(), QObject *parent = 0);
    FastDatabaseRecord(const FastDatabaseRecord& aObject);
    QSqlRecord toRecord() const;
    QVariant operator[](QString aName) const;
    FastDatabaseRecord operator=(const FastDatabaseRecord& aObject);
    void setData(QString aName, QVariant aValue);
private:
    QSqlRecord mResult;
};

class FastDatabaseResult : public QObject
{
    Q_OBJECT
public:
    explicit FastDatabaseResult(QSqlQuery* apResult, QObject *parent = 0);
    FastDatabaseResult(QObject *parent = 0);
    FastDatabaseResult(const FastDatabaseResult& aObject);
    FastDatabaseRecord operator[](int aRow) const;
    FastDatabaseResult operator=(const FastDatabaseResult& aObject);
    int count() const;
    FastDatabaseRecord at(int aRow) const;

    class const_iterator
    {
    public:
        const_iterator(FastDatabaseResult* apResult, int aRow = -1);
        const_iterator(const const_iterator& aObject);
        const FastDatabaseRecord& operator*();
        bool operator!=( const const_iterator& ) const;
        const_iterator& operator++();
        void setRow(int aRow);
        int row() const;
        FastDatabaseResult* data() const;
    private:
        mutable int mRow;
        FastDatabaseResult* mpResult;
        FastDatabaseRecord mCurrent;
    };

    const_iterator begin() const;
    const_iterator end() const;
protected:
    QSqlQuery *data() const;
signals:

public slots:
private:
    QSqlQuery* mpResults;
    int mCount;
    const_iterator mBegin;
    const_iterator mEnd;
    int mRow;
};

#endif // FASTDATABASE_H
