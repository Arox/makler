#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

#include <QTimer>
#include <QCloseEvent>
#include <QScrollArea>

#include "widgetforcontrol.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    enum StatusApp
    {
        INIZIALIZE,
        WORK,
        CLOSE
    };
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int user_fk();
    QStringList roles() const;

public slots:
    void load(WidgetForControl::SignalWidgetType apControl);
    void backWidget(WidgetForControl*);
    void closeWidget();
    void closeWindow();
signals:
    void reloadWidget(WidgetForControl* apControl);
protected:
    void disconnectWidget(int aIndex);
    void connectWidget(int aIndex);
    void setSize(int aIndex);
    bool event(QEvent * event);
    void closeEvent(QCloseEvent *);
protected slots:
    void countMail();
private slots:
    void on_mpHelp_clicked();

    void on_mpInformation_clicked();
    void changeStyle(QString aNameStyle);

    void on_mpSettings_clicked();

private:
    Ui::MainWindow *ui;
    QList<WidgetForControl*> mWidgets;
    int mUser_fk;
    QStringList mRoles;
    QTimer mTimer;
    QScrollArea* mpArea;
    StatusApp mStatus;
};

#endif // MAINWINDOW_H
