#include "styles.h"
#include <QApplication>

#include <QSettings>
#include <QMessageBox>
#include <QResource>

#include "globalsbase.h"

QStringList Styles::mStyleNames;
const QString Styles::vNameDirStyles = "styles";
void Styles::inizialize()
{
    mStyleNames.clear();
    QDir vPath(QApplication::applicationDirPath());
    vPath.setFilter(QDir::Files | QDir::NoSymLinks);
    vPath.setSorting(QDir::Name| QDir::Reversed);
    if (vPath.cd(vNameDirStyles))
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
    if (vPath.cd(vNameDirStyles))
    {
        QSettings vStyleFile(vPath.filePath(aStyleName), QSettings::IniFormat);
        loadStyles(vStyleFile.value("files").toStringList());
        registerResources(vStyleFile.value("resources").toStringList());
    }
    return false;
}


void Styles::loadStyles(QStringList aFiles)
{
    QFile* vpFile = 0;
    QStringList vStyles;
    foreach (QString vFileName, aFiles)
    {
        if (vpFile)
        {
            delete vpFile;
        }
        vpFile = new QFile(vFileName);
        if (vpFile->exists())
        {
            if (vpFile->open(QIODevice::ReadOnly))
            {
                vStyles.append(QString(vpFile->readAll()));
            }
            else
            {
                QMessageBox::warning(0, TRANSLATE("Предупреждение"), QString("%1 %2").arg(TRANSLATE("Не удалось загрузить файл ").arg(vFileName)));
            }
        }
    }
    qApp->setStyleSheet(vStyles.join('\n'));
}

QStringList Styles::mResources;

void Styles::registerResources(QStringList aFiles)
{
    foreach (QString vFileName, Styles::mResources) {
        QResource::unregisterResource(vFileName);
    }
    Styles::mResources.clear();
    foreach (QString vFileName, aFiles) {
        QResource::registerResource(vFileName);
        Styles::mResources.append(vFileName);
    }
}
