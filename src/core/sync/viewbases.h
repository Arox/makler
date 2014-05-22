#ifndef VIEWBASES_H
#define VIEWBASES_H

#include <QWidget>
#include "widgetforcontrol.h"
#include "basestablemodel.h"

namespace Ui {
class ViewBases;
}

class ViewBases : public WidgetForControl
{
    Q_OBJECT

public:
    explicit ViewBases(QWidget *parent = 0);
    ~ViewBases();
    virtual QString name();
    virtual QSize minSize();
    virtual QSize maxSize();
    virtual QSize size();
    virtual QString idName();
private slots:
    void on_mpAdd_clicked();

    void on_mpRemove_clicked();

    void on_mpView_activated(const QModelIndex &index);

    void on_mpView_clicked(const QModelIndex &index);

    void on_mpSync_clicked();

private:
    Ui::ViewBases *ui;
    BasesTableModel mModel;
};

#endif // VIEWBASES_H
