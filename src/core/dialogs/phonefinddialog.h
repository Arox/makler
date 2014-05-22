#ifndef PHONEFINDDIALOG_H
#define PHONEFINDDIALOG_H

#include <QDialog>
#include "address/addresswidget.h"
#include "address/addresshomewidget.h"
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

    void selectCity();
    void selectNoCity();

private:
    void showResults(QString aWhere);
    void showEmptyResult();
private:
    Ui::PhoneFindDialog *ui;
    AddressWidget mAddress;
    AddressHomeWidget mHomeAddress;
    MyLineEdit mFio;
    MyLineEdit mPhone;
    MyLineEdit mNumber;
    int mId;
};

#endif // PHONEFINDDIALOG_H
