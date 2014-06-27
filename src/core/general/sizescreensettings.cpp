#include "sizescreensettings.h"

#include <QDir>
#include <QFile>
#include <QSettings>
#include <QSize>
#include <QMap>
#include <QDesktopWidget>
#include <QApplication>

QString SizeScreenSettings::mDirPath = "settings/screen";

SizeScreenSettings::SizeScreenSettings(QObject *parent) :
    QObject(parent)
{

}

QStringList SizeScreenSettings::filesSettings()
{
    QDir vDir(mDirPath);
    if (vDir.exists())
    {
        return vDir.entryList(QDir::Files);
    }
    return QStringList();
}

QString SizeScreenSettings::screenSettings(QString aFileName)
{
    QSettings vFileSettings(aFileName, QSettings::IniFormat);
    QStringList vSettings;
    foreach (QString vFileStyle, vFileSettings.value("files").toStringList())
    {
        QFile vFile(vFileStyle);
        if (vFile.open(QIODevice::ReadOnly))
        {
            vSettings << vFile.readAll();
        }
    }
    return vSettings.join('\n');
}

void SizeScreenSettings::setScreenSettings(QString aFileName)
{
    qApp->setStyleSheet(screenSettings(aFileName));
}

QString SizeScreenSettings::fileForScreenSize(QSize aSize)
{
    QMap<QString, QSize> vSizes;
    QDir vDir(mDirPath);
    foreach (QString vFileName, filesSettings()) {
         QSettings vFileSettings(vDir.filePath(vFileName), QSettings::IniFormat);
         QSize vSize(vFileSettings.value("width").toInt(), vFileSettings.value("height").toInt());
         if (vSize.width() <= aSize.width())
         {
            vSizes[vFileName] = vSize;
         }
    }
    if (!vSizes.count()) return QString();

    int vMaxIndex = -1;
    int vMax = -1;
    int vMinIndex = -1;
    int vMin = 100000;
    for (int i = 0; i < vSizes.count(); ++i)
    {
        int vCurrent = vSizes.value(vSizes.keys().at(i)).height();
        if (vCurrent <= aSize.height() && vCurrent > vMax)
        {
            vMax= vCurrent;
            vMaxIndex = i;
        }
        if (vCurrent <= aSize.height() && vCurrent < vMin)
        {
            vMin= vCurrent;
            vMinIndex = i;
        }
    }
    if (vMaxIndex >= 0) return vSizes.keys().at(vMaxIndex);
    return vSizes.keys().at(vMinIndex);
}

QString SizeScreenSettings::fileForCurrentScreen()
{
    QDesktopWidget vDesktop;
    return fileForScreenSize(QSize(vDesktop.width(), vDesktop.height()));
}
