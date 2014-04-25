#ifndef DIALOGASIDEUP_H
#define DIALOGASIDEUP_H

#include "globalsbase.h"

#include <QDialog>
#include <QSignalMapper>
#include <QAbstractTableModel>
namespace Ui {
class DialogAsideUp;
}

class ClientsAsideModel : public QAbstractTableModel
{
    Q_OBJECT
protected:
    QStringList mHeaders;
    ResponseType mData;

public:
    ClientsAsideModel(QObject *parent = 0);

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);

    virtual int rowCount(const QModelIndex &parent=QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent=QModelIndex()) const;

    virtual QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    virtual int id(int aRow) const;
    virtual void remove(int aRow);
};

//==========================================================

class DialogAsideUp : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAsideUp(int aUser_fk, QWidget *parent = 0);
    ~DialogAsideUp();
    int countAside();
private slots:
    void on_mpAsides(const QModelIndex &index);
    void toAsideClicked();
    void toActiveClicked(int aRow);
protected:
    void load();
    void toAside(int aId, QDate aDate, QString aComment);
    void toActive(int aId);
private:
    void refreshAsides();
private:
    Ui::DialogAsideUp *ui;
    int mUser_fk;
    ResponseType mData;
    QSignalMapper mMapper;
};

#endif // DIALOGASIDEUP_H
