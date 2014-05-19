#ifndef MYTABLEVIEW_H
#define MYTABLEVIEW_H

#include <QTableView>

class MyTableView : public QTableView
{
    Q_OBJECT
public:
    explicit MyTableView(QWidget *parent = 0);
protected:
    virtual void currentChanged(const QModelIndex & current, const QModelIndex & previous);
    virtual void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
signals:
    void currentIndexChanged(const QModelIndex & current, const QModelIndex & previous);
    void currentSelectionChanged(const QModelIndex & current);
};

#endif // MYTABLEVIEW_H
