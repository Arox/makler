#ifndef PHONEFINDDIALOG_H
#define PHONEFINDDIALOG_H

#include <QDialog>
#include "address/address.h"
#include "mylineedit.h"

namespace Ui {
class PhoneFindDialog;
}

class PhoneFindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PhoneFindDialog(QWidget *parent = 0);
    ~PhoneFindDialog();
    int resultId();
private slots:
    void on_mpFind_clicked();
    void on_mpCancel_clicked();
    void on_mpNext_clicked();
private:
    void showResults(QString aWhere);
    void showEmptyResult();
private:
    Ui::PhoneFindDialog *ui;
    Address mAddress;
    MyLineEdit mFio;
    MyLineEdit mPhone;
    MyLineEdit mNumber;
    int mId;
};

#endif // PHONEFINDDIALOG_H
