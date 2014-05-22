#ifndef VIEWPOSTWIDGET_H
#define VIEWPOSTWIDGET_H

#include <QWidget>
#include "widgetslist.h"

namespace Ui {
class ViewPostWidget;
}

class ViewPostWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ViewPostWidget(int aUser_fk, bool aRemoveElement, QWidget *parent = 0);
    ~ViewPostWidget();
    void addUser(QString aName, int aId);
    void addFile(QString aDisplayFile, QString aFileName);
    QList<int> idUserList();
public slots:
    void save();
    void load(int aIdMessage);
signals:
    void removeUser();
    void removeFile();
    void clickLink(QString aName, QByteArray aArray);
private:
    Ui::ViewPostWidget *ui;
    int mUser_fk;
    WidgetsList mUsers;
    WidgetsList mFiles;
};

#endif // VIEWPOSTWIDGET_H
