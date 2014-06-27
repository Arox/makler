#ifndef SIZESCREENSETTINGS_H
#define SIZESCREENSETTINGS_H

#include <QObject>
#include <QStringList>

class SizeScreenSettings : public QObject
{
    Q_OBJECT
public:
   static QStringList filesSettings();
   static QString screenSettings(QString aFileName);
   static void setScreenSettings(QString aFileName);
   static QString fileForScreenSize(QSize aSize);
   static QString fileForCurrentScreen();
signals:

public slots:
private:
    explicit SizeScreenSettings(QObject *parent = 0);
    static QString mDirPath;
};

#endif // SIZESCREENSETTINGS_H
