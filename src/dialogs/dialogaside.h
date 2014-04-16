#ifndef DIALOGASIDE_H
#define DIALOGASIDE_H

#include <QDialog>

namespace Ui {
class DialogAside;
}

class DialogAside : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogAside(int aIdObject, QWidget *parent = 0);
    ~DialogAside();
    
private slots:
    void on_mpButtonAside_clicked();

    void on_mpButtonCancel_clicked();

private:
    Ui::DialogAside *ui;
    int mIdObject;
};

#endif // DIALOGASIDE_H
