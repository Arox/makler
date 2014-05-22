#ifndef DIALOGAUTHENTIFICATION_H
#define DIALOGAUTHENTIFICATION_H

#include <QDialog>
#include <QStringList>
#include <QFocusEvent>
#include <QPalette>

#include "globalsbase.h"
#include "mylineedit.h"

namespace Ui {
class DialogAuthentification;
}

class DialogAuthentification : public QDialog
{
    Q_OBJECT
    int mId;
public:
    explicit DialogAuthentification(QWidget *parent = 0);
    ~DialogAuthentification();
    int idAgent();
    QStringList roles() const;
private slots:
    void on_mpOk_clicked();

    void on_mpExit_clicked();
private:
    Ui::DialogAuthentification *ui;
    QStringList mRoles;
    MyLineEdit* mpLineLogin;
    MyLineEdit* mpLinePassword;
};

#endif // DIALOGAUTHENTIFICATION_H
