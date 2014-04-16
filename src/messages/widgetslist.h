#ifndef WIDGETSLIST_H
#define WIDGETSLIST_H

#include <QWidget>
#include <QList>
#include <QVariant>
#include <QSignalMapper>

#include "widgetforlist.h"

namespace Ui {
class WidgetsList;
}

class WidgetsList : public QWidget
{
    Q_OBJECT
    static const int gCountButtonsUser;
public:
    explicit WidgetsList(bool aNeedRemove, QWidget *parent = 0);
    ~WidgetsList();
    void add(QString aName, QVariant aData);
    QList<QVariant> data();
    QVariant data(int index);
    int countData();
    void setNeedRemove(bool aNeedRemove);
signals:
    void removeElement();
    void clickLink(QByteArray aData);
protected slots:
    void removeElement(int aIndex);
    void clickLink(int aIndex);
protected:
    void reloadButtonsUser(int aStartIndex = 0);
private:
    Ui::WidgetsList *ui;
    QList<WidgetForList*> mWidgetsList;
    bool mNeedRemove;
    QSignalMapper mMapper;
    QSignalMapper mClickMapper;
};

#endif // WIDGETSLIST_H
