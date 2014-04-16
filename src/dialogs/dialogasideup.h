#ifndef DIALOGASIDEUP_H
#define DIALOGASIDEUP_H

#include "globalsbase.h"

#include <QDialog>

namespace Ui {
class DialogAsideUp;
}

class DialogAsideUp : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAsideUp(int aUser_fk, QWidget *parent = 0);
    ~DialogAsideUp();
    int countAside();
private slots:
    void on_mpListAsides_activated(const QModelIndex &index);

    void on_mpListAsides_itemSelectionChanged();
    void on_mpButtonAside_clicked();

    void on_mpButtonUp_clicked();

protected:
    void load();
private:
    Ui::DialogAsideUp *ui;
    int mUser_fk;
    ResponseType mData;
};

#endif // DIALOGASIDEUP_H
