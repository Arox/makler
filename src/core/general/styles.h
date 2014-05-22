#ifndef STYLES_H
#define STYLES_H

#include <QDir>
#include <QFile>

class Styles: public QObject
{
    Q_OBJECT
    static QStringList mStyleNames;
    static QStringList mResources;
    static const QString vNameDirStyles;
public:
    static void inizialize();
    static QStringList styles();
    static bool setStyle(QString aStyleName);
private:
    static void loadStyles(QStringList aFiles);
    static void registerResources(QStringList aFiles);
};

#endif // STYLES_H