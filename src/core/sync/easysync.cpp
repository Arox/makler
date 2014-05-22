#include "easysync.h"

#include <QVector>
#include <QSqlField>
#include <QStringList>

const int gCountLineSync = 1000;

IdType EasySyncTable::toKey(IdType aKey, int aKeySource)
{
    IdType vKey = fromKey(aKey);
    vKey = vKey | (aKeySource << 24);
    return vKey;
}

IdType EasySyncTable::fromKey(IdType aKey)
{
    return aKey & 0x00FFFFFF;
}

int EasySyncTable::valueKey(IdType aKey)
{
    return aKey & 0xFF000000;
}

IdType EasySyncTable::minId()
{
    return 0;
}

IdType EasySyncTable::maxId()
{
    return 0x00FFFFFF;
}

QMap<int, int> EasySyncTable::matrixOfKeys(QString aSource, QString aDest)
{
    QMap<int, int> vKeys;
    QString vTextQuery = "SELECT id, name, host, user, pass, port FROM bases";
    ResponseType vSourceBases = execQuery(vTextQuery, aSource);
    ResponseType vDestBases = execQuery(vTextQuery, aDest);

    foreach (ResponseRecordType vSourceRecord, vSourceBases) {
        QString vName = vSourceRecord["name"].toString();
        int vDestIndex = -1;
        for (int i = 0; i < vDestBases.count(); ++i)
        {
            if (vName.compare(vDestBases.at(i)["name"].toString()) == 0)
            {
                vDestIndex =  vDestBases.at(i)["id"].toInt();
                break;
            }
        }
        if (vDestIndex < 0)
        {
            execQuery(QString("INSERT INTO bases (name, host, \"user\", pass, port) VALUES ('%1', '%2', '%3', '%4', %4)")
                      .arg(vSourceRecord["name"].toString())
                    .arg(vSourceRecord["host"].toString())
                    .arg(vSourceRecord["user"].toString())
                    .arg(vSourceRecord["pass"].toString())
                    .arg(vSourceRecord["port"].toInt()), aDest);
            vDestIndex = execQuery("SELECT id FROM bases ORDER BY id")[0]["id"].toInt();
        }
        vKeys.insert(vSourceRecord["id"].toInt(), vDestIndex);
    }
    return vKeys;
}

void EasySyncTable::sync(QString aSource, QString aDest, QList< TableInfo > aTables, int aKeySource)
{
    mKeys = matrixOfKeys(aSource, aDest);
    int vCount = aTables.count();
    for(int i = 0; i<vCount; ++i) {
        TableInfo vValue = aTables.at(i);
        deleteData(aSource, aDest, vValue, aKeySource);
        updateData(aSource, aDest, vValue, aKeySource);
        insertData(aSource, aDest, vValue, aKeySource);
    }
}

void EasySyncTable::deleteData(QString aSource, QString aDest, TableInfo aTable, int aKeySource)
{
    ResponseType vResponse;
    if (aTable.mWhere.length())
    {
        vResponse = execQuery(QString("SELECT id FROM %1 WHERE id < %2 AND %3 order by id").arg(aTable.mTableName).arg(maxId()).arg(aTable.mWhere), aSource);
    }
    else
    {
        vResponse = execQuery(QString("SELECT id FROM %1 WHERE id < %2 order by id").arg(aTable.mTableName).arg(maxId()), aSource);
    }

    if (!vResponse.count()) return;
    QVector<bool> vArrayRealId(vResponse.last()["id"].toIdType + 1);

    foreach (ResponseRecordType vRecord, vResponse) {
        vArrayRealId[vRecord["id"].toIdType] = true;
    }

    ResponseType vResponseDest;
    if (aTable.mWhere.length())
    {
        vResponseDest = execQuery(QString("SELECT id FROM %1 WHERE id < %2 AND id > %3 AND %4 order by id").arg(aTable.mTableName).arg(toKey(maxId(), aKeySource)).arg(toKey(minId(), aKeySource)).arg(aTable.mWhere), aDest);
    }
    else
    {
        vResponseDest = execQuery(QString("SELECT id FROM %1 WHERE id < %2 AND id > %3 order by id").arg(aTable.mTableName).arg(toKey(maxId(), aKeySource)).arg(toKey(minId(), aKeySource)), aDest);
    }

    foreach (ResponseRecordType vRecord, vResponseDest) {
        IdType vKey = vRecord["id"].toIdType;
        vKey = fromKey(vKey);
        if (!vArrayRealId[vKey])
        {
            execQuery(QString("DELETE FROM %1 WHERE id = %2").arg(aTable.mTableName).arg(vRecord["id"].toIdType), aDest);
        }
    }

}

void EasySyncTable::updateData(QString aSource, QString aDest, TableInfo aTable, int aKeySource)
{
    ResponseType vResponseDest;
    if (aTable.mWhere.length())
    {
        vResponseDest = execQuery(QString("SELECT id FROM %1 WHERE id < %2 AND id > %3 AND %4 order by id").arg(aTable.mTableName).arg(toKey(maxId(), aKeySource)).arg(toKey(minId(), aKeySource)).arg(aTable.mWhere), aDest);
    }
    else
    {
        vResponseDest = execQuery(QString("SELECT id FROM %1 WHERE id < %2 AND id > %3 order by id").arg(aTable.mTableName).arg(toKey(maxId(), aKeySource)).arg(toKey(minId(), aKeySource)), aDest);
    }

    if (!vResponseDest.count()) return;



    ResponseType vResponse;
    if (aTable.mWhere.length())
    {
        vResponse = execQuery(QString("SELECT * FROM %1 WHERE id < %2 AND %3 order by id").arg(aTable.mTableName).arg(maxId()).arg(aTable.mWhere), aSource);
    }
    else
    {
        vResponse = execQuery(QString("SELECT * FROM %1 WHERE id < %2 order by id").arg(aTable.mTableName).arg(maxId()), aSource);
    }


    if (!vResponse.count()) return;

    QVector<bool> vArrayRealId(toKey(vResponse.last()["id"].toIdType + 1, aKeySource));

    foreach (ResponseRecordType vRecord, vResponseDest) {
        vArrayRealId[vRecord["id"].toIdType] = true;
    }

    QSqlRecord vRecord = vResponse[0].toRecord();

    QStringList vParams;
    int vIdColumn = -1;
    for (int i = 0; i < vRecord.count(); ++i)
    {
        if (!vRecord.fieldName(i).compare(aTable.mIdName))
        {
            vIdColumn = i;
            //continue;
        }
        if ((vRecord.field(i).type() == QVariant::String) || (vRecord.field(i).type() == QVariant::Date) || (vRecord.field(i).type() == QVariant::DateTime) || (vRecord.field(i).type() == QVariant::Time))
        {
            vParams.append(QString(" \"%1\" = '%2',").arg(vRecord.fieldName(i)).arg("%1"));
        }
        else
        {
            vParams.append(QString(" \"%1\" = %2,").arg(vRecord.fieldName(i)).arg("%1"));
        }
    }

    foreach (ResponseRecordType vRecord, vResponse) {
        IdType vKey = vRecord["id"].toIdType;
        vKey = toKey(vKey, aKeySource);
        if (vArrayRealId[vKey])
        {
            QString vQuery = QString("UPDATE %1 SET").arg(aTable.mTableName);
            QSqlRecord vSqlRecord = vRecord.toRecord();
            for (int i = 0; i < vSqlRecord.count(); ++i)
            {
                if (i == vIdColumn) continue;

                if (aTable.mFks.contains(vSqlRecord.fieldName(i)))
                {
                    if (vSqlRecord.field(i).isNull())
                    {
                        vQuery += vParams.at(i).arg("NULL");
                    }
                    IdType vKey = vSqlRecord.value(i).toInt();
                    vQuery += vParams.at(i).arg(toKey(vKey, mKeys[valueKey(aKeySource)]));
                }
                else
                {
                    if (vSqlRecord.field(i).isNull())
                    {
                        vSqlRecord.setValue(i, "NULL");
                    }
                    vQuery += vParams.at(i).arg(vSqlRecord.value(i).toString());
                }
            }
            vQuery.remove(vQuery.length() - 1, 1);
            vQuery += QString(" WHERE id = %1").arg(vKey);
            execQuery(vQuery, aDest);
        }
    }
}

void EasySyncTable::insertData(QString aSource, QString aDest, TableInfo aTable, int aKeySource)
{
    ResponseType vResponseDest;
    if (aTable.mWhere.length())
    {
        vResponseDest = execQuery(QString("SELECT id FROM %1 WHERE id < %2 AND id > %3 AND %4 order by id").arg(aTable.mTableName).arg(toKey(maxId(), aKeySource)).arg(toKey(minId(), aKeySource)).arg(aTable.mWhere), aDest);
    }
    else
    {
        vResponseDest = execQuery(QString("SELECT id FROM %1 WHERE id < %2 AND id > %3 order by id").arg(aTable.mTableName).arg(toKey(maxId(), aKeySource)).arg(toKey(minId(), aKeySource)), aDest);
    }

    ResponseType vResponse;
    if (aTable.mWhere.length())
    {
        vResponse = execQuery(QString("SELECT * FROM %1 WHERE id < %2 AND %3 order by id").arg(aTable.mTableName).arg(maxId()).arg(aTable.mWhere), aSource);
    }
    else
    {
        vResponse = execQuery(QString("SELECT * FROM %1 WHERE id < %2 order by id").arg(aTable.mTableName).arg(maxId()), aSource);
    }


    if (!vResponse.count()) return;

    QVector<bool> vArrayRealId(toKey(vResponse.last()["id"].toIdType + 1, aKeySource));

    foreach (ResponseRecordType vRecord, vResponseDest) {
        vArrayRealId[vRecord["id"].toIdType] = true;
    }

    QSqlRecord vRecord = vResponse[0].toRecord();

    QString vQuery1 = QString("INSERT INTO %1 (").arg(aTable.mTableName);
    for (int i = 0; i < vRecord.count(); ++i)
    {
        vQuery1 += QString(" \"%1\",").arg(vRecord.fieldName(i));
    }
    vQuery1.remove(vQuery1.length() - 1, 1);
    vQuery1 += QString(") VALUES (");

    foreach (ResponseRecordType vRecord, vResponse) {
        IdType vKey = vRecord["id"].toIdType;
        vKey = toKey(vKey, aKeySource);
        if (!vArrayRealId.count() || !vArrayRealId[vKey])
        {
            QString vCopyQuery = vQuery1;
            QSqlRecord vSqlRecord = vRecord.toRecord();
            for (int i = 0; i < vSqlRecord.count(); ++i)
            {

                if (!vSqlRecord.fieldName(i).compare(aTable.mIdName))
                {
                    vCopyQuery += QString(" %1,").arg(vKey);
                }
                else
                {
                    if (aTable.mFks.contains(vSqlRecord.fieldName(i)))
                    {
                        if (vSqlRecord.field(i).isNull())
                        {
                            vCopyQuery += QString(" NULL,");
                        }
                        else
                        {
                            vCopyQuery += QString(" %1,").arg(toKey(vSqlRecord.value(i).toInt(), aKeySource));
                        }
                    }
                    else
                    {
                        if (vSqlRecord.field(i).isNull())
                        {
                            vSqlRecord.setValue(i, "NULL");
                        }
                        if ((vSqlRecord.field(i).type() == QVariant::String) || (vSqlRecord.field(i).type() == QVariant::Date) || (vSqlRecord.field(i).type() == QVariant::DateTime) || (vSqlRecord.field(i).type() == QVariant::Time))
                        {
                            vCopyQuery += QString(" '%1',").arg(vSqlRecord.value(i).toString());
                        }
                        else
                        {
                            vCopyQuery += QString(" %1,").arg(vSqlRecord.value(i).toString());
                        }
                    }
                }
            }
            vCopyQuery.remove(vCopyQuery.length() - 1, 1);
            vCopyQuery += ")";
            execQuery(vCopyQuery, aDest);
        }
    }
}
