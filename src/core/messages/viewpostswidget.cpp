#include "viewpostswidget.h"
#include "ui_viewpostswidget.h"
#include "globals.h"
#include "dialoguniversal.h"
#include "readpostwidget.h"
#include "writepost.h"
#include "language.h"

#include <QApplication>
#include <QColor>
#include <QDateTime>
#include "messages.h"

TableModelPost::TableModelPost(int aIdUser, QObject *parent):
    QAbstractTableModel(parent)
  , mCheckedsColumn(-1)
  , mIdUser(aIdUser)
  , mCountRowInPage(20)
  , mNumberPage(0)
{
    mTimer.setSingleShot(false);
    mTimer.setInterval(10 * 1000);
    connect(&mTimer, SIGNAL(timeout()), this, SLOT(needUpdate()));

    mHeaders << TRANSLATE("получено/отправлено")
             << TRANSLATE("от кого/ кому")
             << TRANSLATE("Дата")
             << TRANSLATE("Время")
             << TRANSLATE("");
    mCheckedsColumn = 0;

    //mSqlStringFull  << QString("select * from messages_full_view where ARRAY[%1] <@ user_fk").arg(mIdUser);
    mSqlStringFull  << QString("select * from full_messages_view where %1 = \"from\"").arg(mIdUser);
    mTimer.start();
}

QVariant TableModelPost::data(const QModelIndex &index, int role) const
{
    ResponseRecordType vRecord;
    QFont vFont;
    int vRow;
    switch (role)
    {
    case Qt::FontRole:
        vRow = rowInDataNumber(index.row());
        vRecord = mData[vRow];
        if (vRecord["in_type"].toString() == "получено")
        {
            if (vRecord["readed"].toBool() == false)
            {

                vFont.setBold(true);
                return vFont;
            }
        }
        return QVariant();
    case Qt::BackgroundColorRole:
        return QVariant();
    case Qt::CheckStateRole:
        if (index.column() == mCheckedsColumn)
        {
            if (checkedId(id(mNumberPage * mCountRowInPage +  index.row())))
            {
                return Qt::Checked;
            }
            else
            {
                return Qt::Unchecked;
            }
        }
        return QVariant();
    case Qt::DisplayRole:
        vRecord = mData[rowInDataNumber(index.row())];
        switch(index.column())
        {
        case 0:
            return vRecord["in_type"];
        case 1:
            return vRecord["name"];
        case 2:
            return vRecord["date_create"].toDateTime().date().toString(DATEFORMAT);
        case 3:
            return vRecord["date_create"].toDateTime().time().toString(TIMEFORMAT);
        case 4:
            return vRecord["message"].toString().left(20);
        }
        return QVariant();
    default:
        return QVariant();
    }
    return QVariant();
}
bool TableModelPost::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column() == mCheckedsColumn)
    {
        if (role == Qt::CheckStateRole)
        {
            if (value.toBool())
            {
                mChecked.append(rowInDataNumber(id(index.row())));
            }
            else
            {
                mChecked.removeAll(rowInDataNumber(id(index.row())));
            }
            emit checkStateChange();
            return true;
        }
    }
    return false;
}

int TableModelPost::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    int vCount = (canNext() ? mCountRowInPage : (fullCount() - mNumberPage * mCountRowInPage));
    return vCount;
}
int TableModelPost::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return  mHeaders.count();
}

QVariant TableModelPost::headerData(int section,Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
               return QVariant();
       if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
       {
           return mHeaders.at(section); // заголовки столбцов
       }
       else
       {
           return QString("%1").arg( section + 1 ); // возвращаем номера строк
       }
}
Qt::ItemFlags TableModelPost::flags(const QModelIndex &index) const
{
        if (index.column() == mCheckedsColumn)
        {
            return QAbstractItemModel::flags(index) | Qt::ItemIsUserCheckable;
        }
       return QAbstractItemModel::flags(index);// | Qt::ItemIsEditable;
}

void TableModelPost::load()
{
    mData = execQuery(mSqlStringFull.join(" "));
}

int TableModelPost::id(int aRow) const
{
    return mData[aRow]["id"].toInt();
}

QList<int> TableModelPost::checkedId()
{
  return mChecked;
}

bool TableModelPost::isChecked()
{
  return mChecked.count();
}

bool TableModelPost::checkedId(int aId) const
{
    return mChecked.contains(aId);
}

void TableModelPost::setColumnChecable(int aColumn)
{
    mCheckedsColumn = aColumn;
}

void TableModelPost::remove(int aId)
{
    ResponseRecordType vRecord;
    int i = 0;
    while ((mData.count() > i) && (mData[i]["id"].toInt() < aId)){++i;}
    if ((mData.count() > i) && (mData[i]["id"].toInt() == aId))
    {
        vRecord = mData[i];
    }
    else
    {
        return;
    }
    if (vRecord["in"].toInt() == 0)
    {
        execQuery(QString("UPDATE messages SET is_active = FALSE WHERE id = %1 AND \"from\" = %2")
                  .arg(vRecord["id_message"].toInt())
                .arg(mIdUser));
    }
    else
    {
        execQuery(QString("UPDATE messages_to SET is_active = FALSE WHERE message_fk = %1 AND  \"to\" = %2")
                  .arg(vRecord["id_message"].toInt())
                .arg(mIdUser));
    }
}

void TableModelPost::setCheckAll()
{
    mChecked.clear();
    for (int i = 0; i < mData.count(); ++i)
    {
        mChecked.append(mData[i]["id"].toInt());
    }
}

void TableModelPost::clearChecked()
{
    mChecked.clear();
}

int TableModelPost::rowInPage()
{
    return mCountRowInPage;
}

int TableModelPost::page()
{
    return mNumberPage;
}

void TableModelPost::setRowInPage(int aCount)
{
    mCountRowInPage = aCount;
}

bool TableModelPost::canNext() const
{
    if (((mNumberPage + 1) * mCountRowInPage) < fullCount())
    {
        return true;
    }
    return false;
}

bool TableModelPost::canPrev() const
{
    if (mNumberPage) return true;
    return false;
}

void TableModelPost::nextPage()
{
    if (canNext())
    {
        mNumberPage++;
    }
}

void TableModelPost::prevPage()
{
    if (canPrev()) --mNumberPage;
}


int TableModelPost::fullCount() const
{
    return mData.count();
}

void TableModelPost::needUpdate()
{
    //ResponseType vResponse = execQuery(QString("SELECT count(*) as count FROM messages_full_view WHERE ARRAY[%1] <@ user_fk").arg(mIdUser));
    ResponseType vResponse = execQuery(QString("SELECT count(*) as count FROM full_messages_view WHERE %1 = \"from\"").arg(mIdUser));
    if (vResponse.count() && (vResponse[0]["count"].toInt() > mData.count()))
    {
        emit update();
    }
}

int TableModelPost::messageId(int aRow)
{
    return mData[rowInDataNumber(aRow)]["id_message"].toInt();
}


int TableModelPost::rowInDataNumber(int aRow) const
{
    return mNumberPage * mCountRowInPage +  aRow;
}







ViewPostsWidget::ViewPostsWidget(int aIdUser, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewPostsWidget),
    mPostModel(aIdUser),
    mUser_fk(aIdUser)
{
    ui->setupUi(this);
    ui->mpWrite->setProperty("color", "true");
    ui->mpRemove->setProperty("remove", "true");
    update();
    connect(&mPostModel, SIGNAL(checkStateChange()), this, SLOT(changeChecked()));
    connect(&mPostModel, SIGNAL(update()), this, SLOT(update()));
}



ViewPostsWidget::~ViewPostsWidget()
{
    delete ui;
}

void ViewPostsWidget::on_mpChecked_clicked()
{
    if (mPostModel.isChecked())
    {
        mPostModel.clearChecked();
    }
    else
    {
        mPostModel.setCheckAll();
    }
    changeChecked();
    ui->mpView->reset();
}

void ViewPostsWidget::changeChecked()
{
    if (mPostModel.isChecked())
    {
        ui->mpChecked->setText(TRANSLATE("Снять выделение"));
    }
    else
    {
        ui->mpChecked->setText(TRANSLATE("Выбрать все"));
    }
}

void ViewPostsWidget::on_mpRemove_clicked()
{
    QList<int> vRemovesId = mPostModel.checkedId();
    if (vRemovesId.count() == 0)
    {
        information(this, TRANSLATE("Невозможно удалить"), TRANSLATE("Вы не выбрали писем, которые необходимо удалить"));
        return;
    }
    if (question(this, TRANSLATE("Вы уверены?"), TRANSLATE("Вы уверены, что хотите удалить письмо?")))
    {
        for (int i = 0; i < vRemovesId.count(); ++i)
        {
            mPostModel.remove(vRemovesId.at(i));
        }
        update();
        mPostModel.clearChecked();
        changeChecked();
    }
}

void ViewPostsWidget::on_mpBack_clicked()
{
    mPostModel.prevPage();
    ui->mpView->reset();
    setTitle();
}

void ViewPostsWidget::on_mpNext_clicked()
{
    mPostModel.nextPage();
    ui->mpView->reset();
    setTitle();
}

void ViewPostsWidget::update()
{
    ui->mpView->setModel(0);
    mPostModel.load();

    setTitle();

    ui->mpView->setModel(&mPostModel);
    ui->mpView->resizeRowsToContents();
    ui->mpView->resizeColumnsToContents();
}

void ViewPostsWidget::setTitle()
{
    int vBegin = mPostModel.fullCount() ? mPostModel.rowInDataNumber(1) : 0;
    int vEnd = mPostModel.canNext() ?  mPostModel.rowInDataNumber(mPostModel.rowInPage()) : mPostModel.fullCount();

    QString vTitle = QString(TRANSLATE("%1-%2 из %3"))
            .arg(vBegin)
            .arg(vEnd)
            .arg(mPostModel.fullCount() ? mPostModel.fullCount() : 0)
            ;

    ui->mpCount->setText(vTitle);
}

void ViewPostsWidget::on_mpView_doubleClicked(const QModelIndex &index)
{
    DialogUniversal* vpDialog = new DialogUniversal(new ReadPostWidget(mUser_fk, mPostModel.messageId(index.row())), TRANSLATE("Сообщение"), this);
    vpDialog->show();
    mPostModel.load();
    ui->mpView->reset();
}

void ViewPostsWidget::on_mpWrite_clicked()
{
    DialogUniversal* vpDialog = new DialogUniversal(new WritePost(mUser_fk, this), TRANSLATE("Написать сообщение"));
    vpDialog->show();
}
