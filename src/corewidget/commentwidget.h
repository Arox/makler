#ifndef COMMENTWIDGET_H
#define COMMENTWIDGET_H

#include <QWidget>

namespace Ui {
class CommentWidget;
}

class CommentWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CommentWidget(QWidget *parent = 0);
    ~CommentWidget();
    void load(int aIdObject);
    void setEnable(bool aEnable);
public slots:
    void save();
private:
    Ui::CommentWidget *ui;
    int mIdObject;
};

#endif // COMMENTWIDGET_H
