#include "mytableview.h"
#include <QHeaderView>

MyTableView::MyTableView(QWidget *parent) :
    QTableView(parent)
{
    this->horizontalHeader()->setStretchLastSection(true);
}

void MyTableView::currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
    QTableView::currentChanged(current, previous);
    emit currentIndexChanged(current, previous);
}

void MyTableView::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
    QTableView::selectionChanged(selected, deselected);
    if (selected.indexes().count()) emit currentSelectionChanged(selected.indexes().last());
}
