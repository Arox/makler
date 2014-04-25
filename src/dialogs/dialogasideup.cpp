#include "dialogasideup.h"
#include "ui_dialogasideup.h"

#include "globals.h"
#include <QSqlQueryModel>
#include <QDebug>

ClientsAsideModel::ClientsAsideModel(QObject *parent):
    QAbstractTableModel(parent)
{
    mHeaders << TRANSLATE("№")          //0
             << TRANSLATE("ФИО")        //1
             << TRANSLATE("Контакт")    //2
             << TRANSLATE("Объект")     //3
             << TRANSLATE("Тут с...")   //4
             << TRANSLATE("");          //5

    mData = execQuery("SELECT id, fio, phone, type, date_create, '' FROM aside_client WHERE date_dead <= now()");
}

QVariant ClientsAsideModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
        switch (index.column())
        {
        case 0:
            return mData[index.row()]["id"];
        case 1:
            return mData[index.row()]["fio"];
        case 2:
            return mData[index.row()]["phone"];
        case 3:
            return mData[index.row()]["type"];
        case 4:
            return mData[index.row()]["date_create"];
        default:
            return QVariant();
        }

    default:
        return QVariant();
    }
    return QVariant();
}
bool ClientsAsideModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

int ClientsAsideModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mData.count();
}
int ClientsAsideModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return  mHeaders.count();
}

QVariant ClientsAsideModel::headerData(int section,Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
               return QVariant();
       if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
       {
           return mHeaders.at(section); // заголовки столбцов
       }

}
Qt::ItemFlags ClientsAsideModel::flags(const QModelIndex &index) const
{
   return QAbstractItemModel::flags(index);// | Qt::ItemIsEditable;
}

int ClientsAsideModel::id(int aRow) const
{
    return mData[aRow]["id"].toInt();
}

void ClientsAsideModel::remove(int aRow)
{
    mData.removeAt(aRow);
}

//==============================================================

DialogAsideUp::DialogAsideUp(int aUser_fk, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAsideUp),
    mUser_fk(aUser_fk)
{
    ui->setupUi(this);
    load();
    ui->mpAsideDescription->setEnabled(false);

    connect(ui->mpAsides, SIGNAL(clicked(QModelIndex)), this, SLOT(on_mpAsides(QModelIndex)));
    connect(ui->mpAside, SIGNAL(clicked()), this,SLOT(toAsideClicked()));
    connect(&mMapper, SIGNAL(mapped(int)), this, SLOT(toActiveClicked(int)));
}

DialogAsideUp::~DialogAsideUp()
{
    delete ui;
}

int DialogAsideUp::countAside()
{
    return ui->mpAsides->model()->rowCount();
}

void DialogAsideUp::load()
{
    ClientsAsideModel* vpModel = new ClientsAsideModel(ui->mpAsides);
    ui->mpAsides->setModel(vpModel);

    QPushButton* vpButton;
    for (int i = 0; i < vpModel->rowCount(); ++i)
    {
        vpButton = new QPushButton(TRANSLATE("восстановить"), ui->mpAsides);
        ui->mpAsides->setIndexWidget(vpModel->index(i,vpModel->columnCount() - 1), vpButton);
        ui->mpAsides->setRowHeight(i, 30);
        connect(vpButton, SIGNAL(clicked()), &mMapper, SLOT(map()));
        mMapper.setMapping(vpButton, i);
    }

    ui->mpAsides->resizeRowsToContents();
    ui->mpAsides->resizeColumnsToContents();
}

void DialogAsideUp::toAside(int aId, QDate aDate, QString aComment)
{
    execQuery(QString("UPDATE aside SET date_dead = '%2', comment = '%3' WHERE id = %1")
              .arg(aId)
            .arg(aDate.toString(DATESAVEFORMAT))
            .arg(aComment));
}

void DialogAsideUp::toActive(int aId)
{
    execQuery(QString("DELETE FROM aside WHERE id = %1").arg(aId));
    execQuery(QString("UPDATE objects SET status = NULL WHERE id = %1").arg(aId));
}

void DialogAsideUp::on_mpAsides(const QModelIndex &index)
{
    ClientsAsideModel* vpModel = dynamic_cast<ClientsAsideModel*>(ui->mpAsides->model());
    Q_ASSERT(vpModel);
    ui->mpAsideDescription->setEnabled(true);
    ResponseType vResponse = execQuery(QString("SELECT comment FROM aside_client WHERE id = %1")
                                 .arg(vpModel->id(index.row())));
    if (vResponse.count())
    {
        ui->mpEditor->clear();
        ui->mpEditor->setPlainText(vResponse[0]["comment"].toString());
        ui->mpCalendar->setCurrentPage(QDate::currentDate().year(), QDate::currentDate().month());
        ui->mpCalendar->setMinimumDate(QDate::currentDate());
    }
}

void DialogAsideUp::refreshAsides()
{
    QAbstractItemModel* vpModel = ui->mpAsides->model();
    ui->mpAsides->setModel(0);
    ui->mpAsides->setModel(vpModel);
    ui->mpAsideDescription->setEnabled(false);

}

void DialogAsideUp::toAsideClicked()
{
    ClientsAsideModel* vpModel = dynamic_cast<ClientsAsideModel*>(ui->mpAsides->model());
    Q_ASSERT(vpModel);
    int vRow = ui->mpAsides->currentIndex().row();
    int vId = vpModel->id(vRow);
    QString vComment = ui->mpEditor->toPlainText();
    QDate vDate = ui->mpCalendar->selectedDate();
    toAside(vId, vDate, vComment);
    vpModel->remove(vRow);
    refreshAsides();
    if (!vpModel->rowCount()) accept();
}

void DialogAsideUp::toActiveClicked(int aRow)
{
    ClientsAsideModel* vpModel = dynamic_cast<ClientsAsideModel*>(ui->mpAsides->model());
    Q_ASSERT(vpModel);
    int vId = vpModel->id(aRow);
    toActive(vId);
    vpModel->remove(aRow);
    refreshAsides();
    if (!vpModel->rowCount()) accept();
}

