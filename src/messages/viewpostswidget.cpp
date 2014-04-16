#include "viewpostswidget.h"
#include "ui_viewpostswidget.h"
#include "globals.h"
#include "dialoguniversal.h"
#include "readpostwidget.h"
#include "writepost.h"

#include <QApplication>
#include <QColor>
#include <QDateTime>


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

    mLastTime = QDateTime(QDate(2000,1,1), QTime(0,0));

    mHeaders << TRANSLATE("получено/отправлено")
             << TRANSLATE("от кого/ кому")
             << TRANSLATE("Дата")
             << TRANSLATE("Время")
             << TRANSLATE("");
    mCheckedsColumn = 0;

    mSqlStringFull  << "select messages.id as id_message, 0 as in, message, \"to\", date_create,"
                    << "mans.sername as sername, mans.name as name,  mans.patronymic as patronymic, messages.readed as readed"
                    << "from messages"
                    << "INNER JOIN messages_to ON messages_to.message_fk = messages.id "
                    << "INNER JOIN users ON users.id = \"to\""
                    << "INNER JOIN mans ON mans.id = users.man_fk"
                    << QString("where \"from\" = %1 AND messages.is_active = TRUE AND messages_to.is_active = TRUE").arg(mIdUser)
                    << "union"
                    << "select messages.id as id_message, 1 as in, message, \"from\", date_create,"
                    << "mans.sername as sername, mans.name as name,  mans.patronymic as patronymic, messages.readed as readed"
                    << "from messages"
                    << "INNER JOIN messages_to ON messages_to.message_fk = messages.id "
                    << "INNER JOIN users ON users.id = \"from\""
                    << "INNER JOIN mans ON mans.id = users.man_fk"
                    << QString("where \"to\" = %1 AND messages.is_active = TRUE AND messages_to.is_active = TRUE").arg(mIdUser)
                    << "order by id_message desc";

    mSqlString      << "SELECT row_number() OVER() as id, * FROM"
                    << "(select messages.id as id_message, 0 as in, message as message, date_create as date_create, messages.readed as readed"
                    << "from messages"
                    << QString("where \"from\" = %1  AND messages.is_active = TRUE").arg(mIdUser)
                    << "union"
                    << "select messages.id as id_message, 1 as in, message as message, date_create as date_create, messages.readed as readed"
                    << "from messages"
                    << "INNER JOIN messages_to ON messages_to.message_fk = messages.id "
                    << QString("where \"to\" = %1 AND messages.is_active = TRUE AND messages_to.is_active = TRUE").arg(mIdUser)
                    << "order by id_message desc) as foo";

    mTimer.start();
}

QVariant TableModelPost::data(const QModelIndex &index, int role) const
{
    QStringList vNames;
    int i = mNumberPage * mCountRowInPage;
    ResponseRecordType vRecord;
    QFont vFont;
    QString vNewName;
    int vRow;
    switch (role)
    {
    case Qt::FontRole:
        vRow = rowInDataNumber(index.row());
        vRecord = mData[vRow];
        if (vRecord["in"].toInt() == 1)
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
            return (vRecord["in"].toInt() == 0 ? TRANSLATE("отправлено") : TRANSLATE("получено"));
        case 1:

            while ((i < mDataFull.count()) && (mDataFull[i]["id_message"].toInt() > vRecord["id_message"].toInt())) { ++i;}
            while ((i < mDataFull.count()) && (mDataFull[i]["id_message"].toInt() == vRecord["id_message"].toInt()))
            {
                vNewName = QString("%1 %2 %3")
                        .arg(mDataFull[i]["sername"].toString())
                        .arg(mDataFull[i]["name"].toString())
                        .arg(mDataFull[i]["patronymic"].toString());
                if (!vNames.contains(vNewName))
                        vNames << vNewName;
                ++i;
            }
            return vNames.join(", \n");
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
                mChecked.append(id(index.row()));
            }
            else
            {
                mChecked.removeAll(id(index.row()));
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
    return (canNext() ? mCountRowInPage : fullCount() % mCountRowInPage);
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
    mDataFull = execQuery(mSqlStringFull.join(" "));
    mData = execQuery(mSqlString.join(" "));
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
    while ((mData.count() > i) && (mData[i]["id"].toInt() > aId)){++i;}
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
    ResponseType vResponse = execQuery(QString("SELECT max(date_create) as date_create FROM messages INNER JOIN messages_to ON messages_to.message_fk = messages.id WHERE \"from\" = %1 OR \"to\" = %1").arg(mIdUser));
    if (vResponse.count())
    {
        ResponseRecordType vRecord = vResponse[0];
        QDateTime vNewTime = vRecord["date_create"].toDateTime();
        QDate vDate1 = vNewTime.date();
        QTime vTime1 = vNewTime.time();
        QDate vDate2 = mLastTime.date();
        QTime vTime2 = mLastTime.time();
        if (vDate1 >= vDate2)
        {
            if((vDate1 > vDate2) || (vTime1 > vTime2))
            {
                mLastTime = vNewTime;
                emit update();
            }
        }
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
    for (int i = 0; i < vRemovesId.count(); ++i)
    {
        mPostModel.remove(vRemovesId.at(i));
    }
    update();
    mPostModel.clearChecked();
    changeChecked();
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
    int vBegin = mPostModel.fullCount() ? mPostModel.page()*mPostModel.rowInPage() + 1 : 0;
    int vEnd = (mPostModel.canNext() ? vBegin + mPostModel.rowInPage() : vBegin + mPostModel.fullCount() % mPostModel.rowInPage());

    QString vTitle = QString(TRANSLATE("%1-%2 из %3"))
            .arg(vBegin)
            .arg(vEnd)
            .arg(mPostModel.fullCount() ? mPostModel.fullCount() + 1 : 0)
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
