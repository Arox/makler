#ifndef STYLES_H
#define STYLES_H

#include <QDir>
#include <QFile>

class Styles: public QObject
{
    Q_OBJECT
    static QStringList mStyleNames;
public:
    static void inizialize();
    static QStringList styles();
    static bool setStyle(QString aStyleName);
};

#endif // STYLES_H
