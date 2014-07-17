#ifndef STYLESWIDGET_H
#define STYLESWIDGET_H

#include <QDialog>

namespace Ui {
class StylesWidget;
}

class StylesWidget : public QDialog
{
    Q_OBJECT

public:
    explicit StylesWidget(QWidget *parent = 0);
    ~StylesWidget();
public slots:
    void changeStyle(QString aNameStyle);
private slots:
    void on_mpOk_clicked();

    void on_mpCancel_clicked();

private:
    Ui::StylesWidget *ui;
};

#endif // STYLESWIDGET_H
