#ifndef WRITEPOST_H
#define WRITEPOST_H

#include <QWidget>
#include "mainwidget.h"
#include "viewpostwidget.h"

namespace Ui {
class WritePost;
}

class WritePost : public MainWidget
{
    Q_OBJECT
public:
    explicit WritePost(int aUser_fk, QWidget *parent = 0);
    ~WritePost();
private slots:

    void on_mpAddFile_clicked();
    void on_mpUsers_activated(int index);
    void loadUsers();
    void on_mpSend_clicked();

private:
    Ui::WritePost *ui;
    ViewPostWidget mViewPost;
    int mUser_fk;
};

#endif // WRITEPOST_H
