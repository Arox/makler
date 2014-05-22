#ifndef VIEWPOSTSWIDGET_H
#define VIEWPOSTSWIDGET_H

#include <QWidget>

namespace Ui {
class ViewPostsWidget;
}

#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QStringList>
#include <QTimer>
#include <QDateTime>

#include "globalsbase.h"

class TableModelPost : public QAbstractTableModel
{
    Q_OBJECT

    int mCheckedsColumn;
    int mIdUser;
    int mCountRowInPage;
    int mNumberPage;

    QStringList mSqlStringFull;

    QTimer mTimer;
protected:
    QStringList mHeaders;
    ResponseType mData;
    QList<int> mChecked;


public:
    TableModelPost(int aIdUser, QObject *parent = 0);

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);

    virtual int rowCount(const QModelIndex &parent=QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent) const;

    virtual QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    virtual void load();

    virtual int id(int aRow) const;


    QList<int> checkedId();
    bool checkedId(int aId) const;
    bool isChecked();
    void setCheckAll();
    void clearChecked();
    void setColumnChecable(int aColumn);

    virtual void remove(int aId);

    int rowInPage();
    int page();
    void setRowInPage(int aCount);

    int fullCount() const;

    bool canNext() const;
    bool canPrev() const;

    int messageId(int aRow);
    int rowInDataNumber(int aRow) const;
protected slots:
    void needUpdate();
public slots:
    void nextPage();
    void prevPage();
signals:
    void checkStateChange();
    void update();
};

class ViewPostsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ViewPostsWidget(int aIdUser, QWidget *parent = 0);
    ~ViewPostsWidget();
    
private slots:
    void on_mpChecked_clicked();
    void changeChecked();
    void on_mpRemove_clicked();

    void on_mpBack_clicked();

    void on_mpNext_clicked();
    void update();
    void setTitle();
    void on_mpView_doubleClicked(const QModelIndex &index);

    void on_mpWrite_clicked();

private:
    Ui::ViewPostsWidget *ui;
    TableModelPost mPostModel;
    int mUser_fk;
};

#endif // VIEWPOSTSWIDGET_H
