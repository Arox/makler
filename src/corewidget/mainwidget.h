#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class MainWidget: public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();
    virtual bool isSave();
signals:
    void back();
protected:
    void load();
    void save();
private:
    bool mIsNew;
};

#endif // MAINWIDGET_H
