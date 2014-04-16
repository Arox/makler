#ifndef DIALOGSHOOT_H
#define DIALOGSHOOT_H

#include <QDialog>

namespace Ui {
class DialogShoot;
}

class DialogShoot : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogShoot(int aIdObject, QWidget *parent = 0);
    ~DialogShoot();
    
private slots:
    void on_mpButtonShoot_clicked();

    void on_mpButtonCancel_clicked();

private:
    Ui::DialogShoot *ui;
    int mIdObject;
};

#endif // DIALOGSHOOT_H
