#include "phonefinddialog.h"
#include "ui_phonefinddialog.h"

#include "globalsbase.h"
#include "language.h"
#include <QSpacerItem>
#include <QSqlQueryModel>

//------------------------------------------------------------------
//----------------------Model for find------------------------------
#include <QAbstractTableModel>
#include <QAbstractItemModel>

#include "globalsbase.h"

class FindPhoneTableModel : public QAbstractTableModel
{
    //Q_OBJECT
protected:
    QStringList mHeaders;
    ResponseType mData;
public:
    FindPhoneTableModel(QObject *parent = 0);

    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual int rowCount(const QModelIndex &parent=QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent) const;

    virtual QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    virtual void load(QString aWhere);
};

FindPhoneTableModel::FindPhoneTableModel(QObject *parent):
    QAbstractTableModel(parent)
{
    mHeaders << TRANSLATE("Агент")          //0
             << TRANSLATE("Тип сделки")    //1
             << TRANSLATE("Имя клиента")      //2
             << TRANSLATE("Адрес")       //3
             << TRANSLATE("Цена")       //4
             << TRANSLATE("Дата")      //5
             ;
}

QVariant FindPhoneTableModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
        if (mHeaders.count() == 1) return TRANSLATE("Результатов не найдено");
        switch(index.column())
        {
        case 0:
            return mData[index.row()]["fio"];
        case 1:
            return mData[index.row()]["type"];
        case 2:
            return mData[index.row()]["client"];
        case 3:
            return QString("   %1   ").arg(mData[index.row()]["address"].toString());
        case 4:
            return QString("%1").arg(QString::number(mData[index.row()]["price"].toDouble(), 'f', 2));
        case 5:
            return mData[index.row()]["date"];
        default:
            return QVariant();
        }
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    default:
        return QVariant();
    }
    return QVariant();
}

int FindPhoneTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (mHeaders.count() == 1) return 1;
    return mData.count();
}

int FindPhoneTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return  mHeaders.count();
}

QVariant FindPhoneTableModel::headerData(int section,Qt::Orientation orientation, int role) const
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
Qt::ItemFlags FindPhoneTableModel::flags(const QModelIndex &index) const
{
       return QAbstractItemModel::flags(index);// | Qt::ItemIsEditable;
}

void FindPhoneTableModel::load(QString aWhere)
{
    QString vSqlQuery;

    if (aWhere.count())
    {
        vSqlQuery = QString("Select fio, (case WHEN type is NULL THEN 'покупатель' ELSE type END) as type, client, address, price, date from total_find_phone_view where %1").arg(aWhere);
    }
    else
    {
        vSqlQuery = QString("Select fio, (case WHEN type is NULL THEN 'покупатель' ELSE type END) as type, client, address, price, date from total_find_phone_view");
    }

    qDebug() << vSqlQuery;
    mData = execQuery(vSqlQuery);

    mHeaders.clear();
    if (mData.count())
    {
        mHeaders << TRANSLATE("Агент")          //0
                 << TRANSLATE("Тип сделки")    //1
                 << TRANSLATE("Имя клиента")      //2
                 << TRANSLATE("Адрес")       //3
                 << TRANSLATE("Цена")       //4
                 << TRANSLATE("Дата")      //5
                 ;
    }
    else
    {
        mHeaders << TRANSLATE("Результат")          //0

                 ;
    }
}

//----------------------End model-----------------------------------
//------------------------------------------------------------------


PhoneFindDialog::PhoneFindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PhoneFindDialog),
    mFio(TRANSLATE("ФИО"), MyLineEdit::Normal),
    mPhone(TRANSLATE("Телефон"), MyLineEdit::Normal),
    mNumber(TRANSLATE("Номер заявки"), MyLineEdit::Normal),
    mId(-1),
    mAddress(location::FINDROOM, this)
{
    ui->setupUi(this);

    ui->contactLayout->insertWidget(0,&mNumber);
    ui->contactLayout->insertWidget(0, &mPhone);
    ui->contactLayout->insertWidget(0, &mFio);

    ui->verticalLayout->insertWidget(1, &mAddress);
}

PhoneFindDialog::~PhoneFindDialog()
{
    delete ui;
}

int PhoneFindDialog::resultId()
{
    return mId;
}

void PhoneFindDialog::showResults(QString aWhere)
{
    if (ui->mpResultView->model())
    {
        delete ui->mpResultView->model();
    }
    ui->mpResultView->setModel(0);
    FindPhoneTableModel* vpModel = new FindPhoneTableModel(this);
    vpModel->load(aWhere);
    ui->mpResultView->setModel(vpModel);
    int vSize = ui->mpResultView->width() / (vpModel->columnCount(QModelIndex()));
    for (int i = 0; i < vpModel->columnCount(QModelIndex()); ++i) ui->mpResultView->setColumnWidth(i, vSize);
    ui->mpResultView->resizeColumnToContents(3);
    ui->mpResultView->resizeColumnToContents(5);
    ui->mpResultView->horizontalHeader()->setStretchLastSection(true);
    ui->mpResultView->show();
}

void PhoneFindDialog::showEmptyResult()
{
    if (ui->mpResultView->model())
    {
        delete ui->mpResultView->model();
    }
    ui->mpResultView->setModel(0);
    FindPhoneTableModel* vpModel = new FindPhoneTableModel(this);
    vpModel->load("false");
    ui->mpResultView->setModel(vpModel);
    ui->mpResultView->horizontalHeader()->setStretchLastSection(true);
    ui->mpResultView->show();
}

void PhoneFindDialog::on_mpFind_clicked()
{
    //ResponseType vResponse;
    QStringList vConstraints;
    if (mNumber.text().length())
    {
        vConstraints << QString("id = %1").arg(mNumber.text());
    }

    //ADD FIO OR PHONE
    QStringList vContactQueryPart;
    if (mPhone.text().length() > 0)
    {
        vContactQueryPart << QString("phone like '%%1%'").arg(mPhone.text());
    }
    if (mFio.text().length())
    {
        vContactQueryPart << QString("client like '%%1%'").arg(mFio.text());
    }
    if (vContactQueryPart.count())
    {
        vConstraints << QString("(%1)").arg(vContactQueryPart.join(" OR "));
    }

    //ADD ADDRESS
    QStringList vWheres;
    vWheres
            << (mAddress.data().isLocality() ? QString("microdistrict_fk = %1").arg(mAddress.data().locality().at(0)) : QString(""))
            << (mAddress.data().isStreet() ? QString("street_fk = %1").arg(mAddress.data().street().at(0)) : QString(""))
            << (mAddress.data().isNumber1() ? QString("number1 = '%1'").arg(mAddress.data().number1()) : QString(""))
            << (mAddress.data().isNumber2() ? QString("number2 = '%1'").arg(mAddress.data().number2()) : QString(""))
            << (mAddress.data().isRoom() ? QString("room = '%1'").arg(mAddress.data().room()) : QString(""));
    vWheres.removeAll("");
    if (vWheres.count())
    {
        vConstraints << QString("(%1)").arg(vWheres.join(" AND "));
    }
    if (vConstraints.count())
    {
        showResults(vConstraints.join(" AND "));
        return;
    }
    showEmptyResult();
}

void PhoneFindDialog::on_mpCancel_clicked()
{
    reject();
}

void PhoneFindDialog::on_mpNext_clicked()
{
    accept();
}
