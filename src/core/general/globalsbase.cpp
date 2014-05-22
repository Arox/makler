#include "globalsbase.h"

#include <QSqlRecord>

ResponseType execQuery(QString aQueryText, QString aBaseName)
{
    QSqlDatabase vDB = QSqlDatabase::database(aBaseName);
    QSqlQuery vQuery(aQueryText, vDB);
    if (vQuery.lastError().isValid())
    {
        qDebug() << vQuery.lastQuery();
        qDebug() << vQuery.lastError().text();
    }
    ResponseType vResult;
    while (vQuery.next())
    {
        vResult.append(FastDatabaseRecord(vQuery.record()));
    }
    return vResult;
}

void markQuery(int aId, QString aTableName, TypeMarkQuery aType)
{
    QString vTypeMark;
    switch (aType) {
    case INSERT:
        vTypeMark = "insert";
        break;
    case UPDATE:
        vTypeMark = "update";
        break;
    default:
        vTypeMark = "delete";
        break;
    }
    execQuery(QString("INSERT INTO journal (link_id, tablename, type, date_change) VALUES (%1, '%2', '%3', now())")
              .arg(aId).arg(aTableName).arg(vTypeMark));
}
