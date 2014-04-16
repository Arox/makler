#ifndef EASYSYNC_H
#define EASYSYNC_H
#include "globalsbase.h"
#include <QSqlDatabase>
#include <QList>
#include <QPair>
#include <QMap>

#define IdType int
#define toIdType toInt()

struct  TableInfo
{
public:
    QString mTableName;
    QString mWhere;
    QString mIdName;
    QList<QString> mFks;
    TableInfo(QString aTableName, QString aWhere, QString aIdName, QList<QString> aFks)
    {
        mTableName = aTableName;
        mWhere = aWhere;
        mIdName = aIdName;
        mFks = aFks;
    }
};

class EasySyncTable
{
private:
    static IdType toKey(IdType aKey, int aKeySource);
    static IdType fromKey(IdType aKey);
    static int valueKey(IdType aKey);

    QMap<int, int> mKeys;
protected:
    QMap<int, int> matrixOfKeys(QString aSource, QString aDest);

    void deleteData(QString aSource, QString aDest, TableInfo aTable, int aKeySource);
    void updateData(QString aSource, QString aDest, TableInfo aTable, int aKeySource);
    void insertData(QString aSource, QString aDest, TableInfo aTable, int aKeySource);
public:
    void sync(QString aSource, QString aDest, QList< TableInfo > aTables, int aKeySource);

    static IdType minId();
    static IdType maxId();
};



#endif // EASYSYNC_H
