#include "styles.h"
#include <QApplication>

QStringList Styles::mStyleNames;

void Styles::inizialize()
{
    mStyleNames.clear();
    QDir vPath(QApplication::applicationDirPath());
    vPath.setFilter(QDir::Files | QDir::NoSymLinks);
    vPath.setSorting(QDir::Name| QDir::Reversed);
    if (vPath.cd("styles"))
    {
        mStyleNames = vPath.entryList();
    }
}

QStringList Styles::styles()
{
    return mStyleNames;
}

bool Styles::setStyle(QString aStyleName)
{
    QDir vPath(QApplication::applicationDirPath());
    if (vPath.cd("styles"))
    {
        QFile vFile(vPath.absoluteFilePath(aStyleName));
        if (vFile.open(QIODevice::ReadOnly))
        {
            QString vStyle = QString(vFile.readAll());
            qApp->setStyleSheet(vStyle);
            return true;
        }
    }
    return false;
}
