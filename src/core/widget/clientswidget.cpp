#include "clientswidget.h"
#include "ui_clientswidget.h"

#include "globalsbase.h"
#include "language.h"
#include "dialogaside.h"
#include "dialogshoot.h"

#include "findapartment.h"
#include "findrent.h"
#include "findhome.h"

#include "apartmentwidget.h"
#include "rentwidget.h"
#include "homewidget.h"

#include <QMessageBox>
#include <QTime>
#include <QInputDialog>
#include "dialoguniversal.h"

ClientsWidget::ClientsWidget(int aAgent, QWidget *parent) :
    WidgetForControl(parent),
    ui(new Ui::ClientsWidget),
    mMapper(this),
    mAgent(aAgent),
    mView(this)
{
    ui->setupUi(this);

    ResponseType vResponse = execQuery(QString("SELECT id FROM static_dictionaries WHERE parent is NULL AND name = 'typeobject'"));
    if (vResponse.count())
    {
        int vParentId = vResponse[0]["id"].toInt();
        vResponse = execQuery(QString("SELECT id, name FROM static_dictionaries WHERE parent = %1").arg(vParentId));
        for (int i = 0; i < vResponse.count(); ++i)
        {
            mValuesTypeObject.append(QPair<QString, int>(vResponse[i]["name"].toString(), vResponse[i]["id"].toInt()));
        }
    }

    ui->verticalLayout->insertWidget(1, &mView);
    connect(&mView, SIGNAL(activated(QModelIndex)), this, SLOT(on_mpView_activated(QModelIndex)));
    connect(&mView, SIGNAL(clicked(QModelIndex)), this, SLOT(on_mpView_clicked(QModelIndex)));
    connect(&mView, SIGNAL(currentSelectionChanged(QModelIndex)), this, SLOT(on_mpView_clicked(QModelIndex)));

    mClientModel.addFilter(QString("agent_fk = %1").arg(aAgent));
    mClientWidget.setVisible(false);
    ui->verticalLayout_2->addWidget(&mFindObject);

    ui->mpButtonActive->setObjectName("active");
    ui->mpButtonDeferred->setObjectName("deffered");
    ui->mpButtonArchive->setObjectName("archive");

    mClientModel.setActive(true);
    mClientModel.setArchive(false);
    mClientModel.setAside(false);

    connect(ui->mpButtonActive, SIGNAL(clicked()), this, SLOT(setButton()));
    connect(ui->mpButtonDeferred, SIGNAL(clicked()), this, SLOT(setButton()));
    connect(ui->mpButtonArchive, SIGNAL(clicked()), this, SLOT(setButton()));

    connect(ui->mpButtonAdd, SIGNAL(clicked()), this, SLOT(add()));
    connect(&mClientWidget, SIGNAL(changed()), this, SLOT(loadView()));

    connect(&mFindObject, SIGNAL(find(FindObjectsWidget::TypeObject)), this, SLOT(findObject(FindObjectsWidget::TypeObject)));
    connect(&mFindObject, SIGNAL(activateObject(FindObjectsWidget::TypeObject,int)), this, SLOT(activateObject(FindObjectsWidget::TypeObject,int)));
    connect(&mClientModel, SIGNAL(changeData(QModelIndex)), this, SLOT(reloadView()));
    reloadView();

    mFindObject.setEnabled(false);

    mView.setObjectName("viewclient");
    mFindObject.setObjectName("viewresultclient");
}

ClientsWidget::~ClientsWidget()
{
    delete ui;
}
void ClientsWidget::add()
{
    bool ok;
    QString vPhone = QInputDialog::getText(this, TRANSLATE("Поиск по номеру телефона"), TRANSLATE("Введите номер телефона"), QLineEdit::Normal, QString(), &ok);
    if (!ok) return;
    if (vPhone.length() == 0)
    {
        QMessageBox::warning(this, TRANSLATE("Ошибка"), TRANSLATE("Вы не ввели номер телефона"));
        return;
    }
    ResponseType vResponse = execQuery(QString("SELECT * FROM clients WHERE phone LIKE '%1'").arg(vPhone));
    if (vResponse.count())
    {
        QMessageBox::warning(this, TRANSLATE("Ошибка"), TRANSLATE("Такой телефон уже есть"));
        return;
    }
    int vTypeId = execQuery(QString("SELECT id FROM types WHERE type = 'client'"))[0]["id"].toInt();
    qsrand(QTime::currentTime().msec());
    int vComment = qrand() % 10000000;
    execQuery(QString("insert into objects (type_fk, \"create\", comment, agent_fk) values (%1, now(), '%2', %3)")
              .arg(vTypeId)
              .arg(vComment)
              .arg(mAgent));
    int vId = execQuery(QString("SELECT id FROM objects WHERE type_fk = %1 AND comment = '%2'")
                    .arg(vTypeId)
                    .arg(vComment))[0]["id"].toInt();
    execQuery(QString("UPDATE objects SET comment = '' WHERE id = %1")
              .arg(vId));
    execQuery(QString("insert into clientheader (object_fk) VALUES (%1) ").arg(vId));
    int vIdClient = execQuery(QString("select id from clientheader where object_fk = %1").arg(vId))
            [0]["id"].toInt();
    execQuery(QString("insert into clients (fio, phone, header_fk) VALUES ('%1', '%2', %3) ")
              .arg(TRANSLATE("НОВЫЙ"))
              .arg(vPhone)
              .arg(vIdClient));
    execQuery(QString("insert into orders (object_fk) VALUES (%1)").arg(vId));
    load();
}

void ClientsWidget::on_mpButtonAdd_clicked()
{

}

void ClientsWidget::on_mpButtonActive_clicked()
{
    ui->mpButtonActive->setChecked(true);
    ui->mpButtonDeferred->setChecked(false);
    ui->mpButtonArchive->setChecked(false);
    mClientModel.setActive(true);
    mClientModel.setArchive(false);
    mClientModel.setAside(false);
    reloadView();
}

void ClientsWidget::on_mpButtonDeferred_clicked()
{
    ui->mpButtonActive->setChecked(false);
    ui->mpButtonDeferred->setChecked(true);
    ui->mpButtonArchive->setChecked(false);
    mClientModel.setAside(true);
    mClientModel.setArchive(false);
    mClientModel.setActive(false);

    reloadView();
}

void ClientsWidget::on_mpButtonArchive_clicked()
{
    ui->mpButtonActive->setChecked(false);
    ui->mpButtonDeferred->setChecked(false);
    ui->mpButtonArchive->setChecked(true);
    mClientModel.setArchive(true);
    mClientModel.setActive(false);
    mClientModel.setAside(false);

    reloadView();
}

bool ClientsWidget::isActive(int aRow)
{
    QString vStatus = mClientModel.status(aRow);
    if (vStatus.isEmpty()) return true;
    if (vStatus == "A")
    {
        QMessageBox::warning(this, TRANSLATE("Невозможно выполнить операцию"), TRANSLATE("Объект уже отложен"));
    }
    else
    {
        if (vStatus == "S")
        {
            QMessageBox::warning(this, TRANSLATE("Невозможно выполнить операцию"), TRANSLATE("Объект уже в архиве"));
        }
        else
        {
            QMessageBox::warning(this, TRANSLATE("Невозможно выполнить операцию"), TRANSLATE("Неизвестное состояние объекта"));
        }
    }
    return false;
}

void ClientsWidget::on_mpButtonAside_clicked()
{
    if (mView.currentIndex().row() >= 0)
    {
        int vIdObject = mClientModel.id(mView.currentIndex().row());
        if (isActive(mView.currentIndex().row()))
        {
            DialogAside vDialog(vIdObject, this);
            vDialog.exec();
            load();
        }
    }
}

void ClientsWidget::on_mpButtonShoot_clicked()
{
    if (mView.currentIndex().row() >= 0)
    {
        int vIdObject = mClientModel.id(mView.currentIndex().row());
        if (isActive(mView.currentIndex().row()))
        {
            DialogShoot vDialog(vIdObject, this);
            vDialog.exec();
            load();
        }
    }
}

void ClientsWidget::setButton()
{
    int vCount = mClientModel.columnCount(QModelIndex());
    foreach (QPushButton* vpOldButton, mListButtonsTable)
    {
        mMapper.removeMappings(vpOldButton);
        delete vpOldButton;
    }
    mListButtonsTable.clear();
    QPushButton* vpButton;
    for (int i = 0; i < mClientModel.rowCount(); ++i)
    {
        if (!mClientModel.status(i).isEmpty())
        {
            vpButton = new QPushButton(TRANSLATE("восстановить"), &mView);
            mView.setIndexWidget(mClientModel.index(i,vCount - 1), vpButton);
            connect(vpButton, SIGNAL(clicked()), &mMapper, SLOT(map()));
            mMapper.setMapping(vpButton, i);
            mListButtonsTable.append(vpButton);
        }
    }
    connect(&mMapper, SIGNAL(mapped(int)), this, SLOT(reloadObject(int)));
}

void ClientsWidget::setComboBox()
{
    foreach (QComboBox* vpOldComboBox, mListBoxesTable)
    {
        mMapper.removeMappings(vpOldComboBox);
        delete vpOldComboBox;
    }
    mListBoxesTable.clear();
    QComboBox* vpBox;

    for (int i = 0; i < mClientModel.rowCount(); ++i)
    {
        vpBox = new QComboBox(&mView);
        vpBox->addItem(TRANSLATE("не выбрано"), -1);
        mView.setIndexWidget(mClientModel.index(i, 3), vpBox);
        connect(vpBox, SIGNAL(currentIndexChanged(int)), &mMapperBoxes, SLOT(map()));
        mMapperBoxes.setMapping(vpBox, i);
        mListBoxesTable.append(vpBox);
        for (int j =0; j < mValuesTypeObject.count(); ++j)
        {
            QPair<QString, int> vItem = mValuesTypeObject.at(j);
            vpBox->addItem(vItem.first, vItem.second);
        }
        if (mClientModel.typeObject(i) > 0)
        {
            vpBox->setCurrentIndex(vpBox->findData(mClientModel.typeObject(i)));
        }
    }
    connect(&mMapperBoxes, SIGNAL(mapped(int)), this, SLOT(changeTypeObject(int)));
}

void ClientsWidget::changeTypeObject(int aRow)
{
    int vId = mClientModel.id(aRow);
    QComboBox* vpBox = mListBoxesTable.at(aRow);
    QString vType = (vpBox->currentIndex() > 0 ? vpBox->itemData(vpBox->currentIndex()).toString() : "NULL");
    execQuery(QString("UPDATE orders SET type_fk = %1 WHERE object_fk = %2")
              .arg(vType)
              .arg(vId));
}

void ClientsWidget::load(const QModelIndex &index)
{
    mClientModel.load();
    if (index.isValid())
    {
        mView.update(index);
    }
    else
    {
        reloadView();
    }
}

void ClientsWidget::reloadObject(int aRow)
{
    QString vStatus = mClientModel.status(aRow);
    if (vStatus.isEmpty()) return;
    int vIdObject = mClientModel.id(aRow);
    if (vStatus == "A")
    {
        execQuery(QString("DELETE FROM aside WHERE id = %1").arg(vIdObject));
    }
    if (vStatus == "S")
    {
        execQuery(QString("DELETE FROM shoot WHERE id = %1").arg(vIdObject));
    }
    execQuery(QString("UPDATE objects SET status = NULL WHERE id = %1").arg(vIdObject));
    load();
}

void ClientsWidget::on_mpView_clicked(const QModelIndex &index)
{
    mFindObject.setObjectId(mClientModel.id(index.row()));
    if (!mFindObject.isEnabled()) mFindObject.setEnabled(true);
    if (index.column() > 1)
    {
        return;
    }
    mView.blockSignals(true);
    mView.setCurrentIndex(index);
    mView.blockSignals(false);
}

void ClientsWidget::on_mpView_activated(const QModelIndex &index)
{
    on_mpView_clicked(index);
    ClientHeaderAlternativeWidget* vpWidget = new ClientHeaderAlternativeWidget(this);
    vpWidget->load(mAgent, mClientModel.id(index.row()));
    //if (!mClientWidget.isVisible()) mClientWidget.setVisible(true);
    //mClientWidget.load(mAgent, mClientModel.id(index.row()));

    DialogUniversal vDialog(vpWidget, "", this);
    vDialog.exec();
    reloadView();
}
void ClientsWidget::loadView()
{
    QModelIndex vIndex = mView.currentIndex();
    load();
    mView.blockSignals(true);
    mView.setCurrentIndex(vIndex);
    mView.blockSignals(false);
}

void ClientsWidget::findObject(FindObjectsWidget::TypeObject aObject)
{
    WidgetForControl* vpWidget = 0;
    switch (aObject)
    {
    case FindObjectsWidget::APARTMENT:
        vpWidget = new FindApartment();
        break;
    case FindObjectsWidget::HOME:
        vpWidget = new FindHome();
        break;
    case FindObjectsWidget::RENT:
        vpWidget = new FindRent();
        break;
    default:
        break;
    }
    if (vpWidget) emit changeWidget(SignalWidgetType(this, vpWidget));
}

void ClientsWidget::reload(WidgetForControl* apControlFind)
{
    FindApartment* vpFindApartment = dynamic_cast<FindApartment*>(apControlFind);
    FindRent * vpFindRent = dynamic_cast<FindRent*>(apControlFind);
    FindHome * vpFindHome = dynamic_cast<FindHome*>(apControlFind);
    if (vpFindApartment)
    {
        mFindObject.load(FindObjectsWidget::APARTMENT, vpFindApartment->sql());
    }
    if (vpFindRent)
    {
        mFindObject.load(FindObjectsWidget::RENT, vpFindRent->sql());
    }
    if (vpFindHome)
    {
        mFindObject.load(FindObjectsWidget::HOME, vpFindHome->sql());
    }
    delete apControlFind;
}

void ClientsWidget::reloadView()
{
    mView.setModel(0);
    mClientModel.load();
    mView.setModel(&mClientModel);
    mView.resizeColumnsToContents();
    mView.resizeRowsToContents();
    mView.resizeColumnToContents(mClientModel.columnCount(QModelIndex()));
}

QString ClientsWidget::name()
{
    return TRANSLATE("Покупатели");
}

QSize ClientsWidget::minSize()
{
    return QSize(0, 0);
}

QSize ClientsWidget::size()
{
    return QSize(1073, 837);
}

QSize ClientsWidget::maxSize()
{
    return QSize(0, 0);
}

QString ClientsWidget::idName()
{
    return "clients";
}

void ClientsWidget::activateObject(FindObjectsWidget::TypeObject aType, int aId)
{
    GeneralWidget* vpWidget = 0;
    switch (aType) {
    case FindObjectsWidget::APARTMENT:
        vpWidget = new ApartmentWidget(mAgent);
        break;
    case FindObjectsWidget::RENT:
        vpWidget = new RentWidget(mAgent);
        break;
    case FindObjectsWidget::HOME:
        vpWidget = new HomeWidget(mAgent);
        break;
    default:
        break;
    }
    if (vpWidget)
    {
        vpWidget->load(aId);
        changeWidget(SignalWidgetType(this, vpWidget));
    }
}

void ClientsWidget::on_mpFilterPhone_textChanged(const QString &arg1)
{
    mClientModel.addFilter(QString("phone like '%%1%'").arg(arg1));
    reloadView();
}

void ClientsWidget::on_mpClearFilter_clicked()
{
    ui->mpFilterPhone->setText("");
}
