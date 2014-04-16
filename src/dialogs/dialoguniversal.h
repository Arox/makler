#ifndef DIALOGUNIVERSAL_H
#define DIALOGUNIVERSAL_H

#include <QDialog>

namespace Ui {
class DialogUniversal;
}

class DialogUniversal : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogUniversal(QWidget* apChildren, QString aName, QWidget *parent = 0);
    ~DialogUniversal();
    
private:
    Ui::DialogUniversal *ui;
};

#endif // DIALOGUNIVERSAL_H
