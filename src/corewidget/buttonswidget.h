#ifndef BUTTONSWIDGET_H
#define BUTTONSWIDGET_H

#include <QWidget>

namespace Ui {
class ButtonsWidget;
}

class ButtonsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ButtonsWidget(QWidget *parent = 0);
    ~ButtonsWidget();
    void setId(int aIdObject);
private slots:
    void on_mpPhoto_clicked();
    void on_mpBack_clicked();

    void on_mpOrder_clicked();

    void on_mpNoSave_clicked();

signals:
    void back();
    void noSave();
private:
    Ui::ButtonsWidget *ui;
    int mIdObject;
};

#endif // BUTTONSWIDGET_H
