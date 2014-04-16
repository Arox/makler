#ifndef WIDGETFORLIST_H
#define WIDGETFORLIST_H

#include <QWidget>
#include <QVariant>
namespace Ui {
class WidgetForList;
}

class WidgetForList : public QWidget
{
    Q_OBJECT
    
public:
    explicit WidgetForList(QString aName, QVariant aData, bool aNeedRemove = true, QWidget *parent = 0);
    ~WidgetForList();
    QVariant data();

signals:
    void remove();
    void clickLink();
private:
    Ui::WidgetForList *ui;
    QVariant mData;
};

#endif // WIDGETFORLIST_H
