#ifndef GLOBALSBASE_H
#define GLOBALSBASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "fastdatabase.h"

#include <QDebug>
#include <QList>
#include <QVector>
#include <QMap>
#include <QVariant>
#include <QApplication>

//#define ResponseRecordType QMap<QString, QVariant>
//#define ResponseType QList< ResponseRecordType >

//#define ResponseRecordType FastDatabaseRecord
//#define ResponseType FastDatabaseResult

#define ResponseRecordType FastDatabaseRecord
#define ResponseType QVector< ResponseRecordType >

ResponseType execQuery(QString aQueryText, QString aBaseName = "local");

enum TypeMarkQuery
{
    INSERT = 0,
    UPDATE = 1,
    DELETE = 2
};

void markQuery(int aId, QString aTableName, TypeMarkQuery aType);
#endif // GLOBALSBASE_H
