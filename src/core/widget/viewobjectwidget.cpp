#include "viewobjectwidget.h"
#include "ui_viewobjectwidget.h"
#include "dialoguniversal.h"
#include "dialogaside.h"
#include "dialogshoot.h"
#include "phonefinddialog.h"
#include "language.h"

#include "tablemodelapartment.h"
#include "tablemodelhome.h"
#include "tablemodelrent.h"

#include "apartmentwidget.h"
#include "homewidget.h"
#include "rentwidget.h"

#include "findapartment.h"
#include "findhome.h"
#include "findrent.h"

#include "messages.h"

ViewObjectWidget::ViewObjectWidget(TypeObject aType, int aAgent_fk, QWidget *parent):
    WidgetForControl(parent),
    ui(new Ui::ViewObjectWidget),
    mMapper(this)
{
    ui->setupUi(this);
    ui->mpButtonAdd->setProperty("color", "true");
    ui->mpFilter->setProperty("color", "true");
    switch (aType) {
    case APARTMENT:
        mpModel = new TableModelApartment();
        mpWidgetAdd = new ApartmentWidget(aAgent_fk);
        mpWidgetFind = new FindApartment();
        break;
    case HOME:
        mpModel = new TableModelHome();
        mpWidgetAdd = new HomeWidget(aAgent_fk);
        mpWidgetFind = new FindHome();
        break;
    case RENT:
        mpModel = new TableModelRent();
        mpWidgetAdd = new RentWidget(aAgent_fk);
        mpWidgetFind = new FindRent();
        break;
    default:
        break;
    }
    QList<QPushButton*> vListButtons;
    vListButtons << ui->mpButtonActive << ui->mpButtonArchive << ui->mpButtonDeferred;

    ui->mpButtonActive->setObjectName("active");
    ui->mpButtonDeferred->setObjectName("deffered");
    ui->mpButtonArchive->setObjectName("archive");

    mpModel->setActive(true);
    mpModel->setAside(false);
    mpModel->setArchive(false);
    foreach (QPushButton* vpButton, vListButtons)
    {
        vpButton->setChecked(false);
    }
    ui->mpButtonActive->setChecked(true);

    load();
    connect(&mMapper, SIGNAL(mapped(int)), this, SLOT(reloadObject(int)));
    connect(ui->mpFilter, SIGNAL(clicked()), this, SLOT(clickFilter()));
    connect(ui->mpClearFilter, SIGNAL(clicked()), this, SLOT(clickClearFilter()));

    ui->mpClearFilter->setVisible(false);
    ui->mpView->setProperty("find", "state1");
}

ViewObjectWidget::~ViewObjectWidget()
{
    delete ui;
    if (mpModel) delete mpModel;
    if (mpWidgetAdd) delete mpWidgetAdd;
    if (mpWidgetFind) delete mpWidgetFind;
}

void ViewObjectWidget::on_mpButtonActive_clicked()
{
    ui->mpButtonActive->setChecked(true);
    ui->mpButtonDeferred->setChecked(false);
    ui->mpButtonArchive->setChecked(false);
    mpModel->setActive(true);
    mpModel->setArchive(false);
    mpModel->setAside(false);
    load();
}

void ViewObjectWidget::on_mpButtonDeferred_clicked()
{
    ui->mpButtonActive->setChecked(false);
    ui->mpButtonDeferred->setChecked(true);
    ui->mpButtonArchive->setChecked(false);
    mpModel->setAside(true);
    mpModel->setArchive(false);
    mpModel->setActive(false);

    load();
}

void ViewObjectWidget::on_mpButtonArchive_clicked()
{
    ui->mpButtonActive->setChecked(false);
    ui->mpButtonDeferred->setChecked(false);
    ui->mpButtonArchive->setChecked(true);
    mpModel->setArchive(true);
    mpModel->setActive(false);
    mpModel->setAside(false);

    load();
}

void ViewObjectWidget::reload(WidgetForControl* apControl)
{
    Q_UNUSED(apControl);
    if (mpWidgetFind == apControl)
    {
        mpModel->addFilter(mpWidgetFind->sql());
        if (mpWidgetFind->sql().isEmpty())
        {
            ui->mpView->setProperty("find", "state1");
        }
        else
        {
            ui->mpClearFilter->setVisible(true);
            ui->mpView->setProperty("find", "state2");
        }
    }

    load();
}

void ViewObjectWidget::on_mpView_doubleClicked(const QModelIndex &index)
{
    mpWidgetAdd->load(mpModel->id(index.row()));
    mCurrentIndex = index;
    mpWidgetAdd->setVisible(true);
    emit changeWidget(SignalWidgetType(this, mpWidgetAdd));
}

void ViewObjectWidget::on_mpButtonAdd_clicked()
{
    if (mpWidgetAdd)
    {
        if (mDialog.exec() == QDialog::Accepted)
        {
            mpWidgetAdd->load(mDialog.resultId());
            emit changeWidget(SignalWidgetType(this, mpWidgetAdd));
        }
        else
        {
            return;
        }
    }
    else
    {
        emit add();
    }
    load();
}

bool ViewObjectWidget::isActive(int aRow)
{
    QString vStatus = mpModel->status(aRow);
    if (vStatus.isEmpty()) return true;
    if (vStatus == "A")
    {
        warning(this, TRANSLATE("Невозможно выполнить операцию"), TRANSLATE("Объект уже отложен"));
    }
    else
    {
        if (vStatus == "S")
        {
            warning(this, TRANSLATE("Невозможно выполнить операцию"), TRANSLATE("Объект уже в архиве"));
        }
        else
        {
            warning(this, TRANSLATE("Невозможно выполнить операцию"), TRANSLATE("Неизвестное состояние объекта"));
        }
    }
    return false;
}

void ViewObjectWidget::on_mpButtonAside_clicked()
{
    if (ui->mpView->currentIndex().row() >= 0)
    {
        int vIdObject = mpModel->id(ui->mpView->currentIndex().row());
        if (isActive(ui->mpView->currentIndex().row()))
        {
            DialogAside vDialog(vIdObject, this);
            vDialog.exec();
            load();
        }
    }
}

void ViewObjectWidget::on_mpButtonShoot_clicked()
{
    if (ui->mpView->currentIndex().row() >= 0)
    {
        int vIdObject = mpModel->id(ui->mpView->currentIndex().row());
        if (isActive(ui->mpView->currentIndex().row()))
        {
            DialogShoot vDialog(vIdObject, this);
            vDialog.exec();
            load();
        }
    }
}

void ViewObjectWidget::setButton()
{
    int vCount = mpModel->columnCount(QModelIndex());
    foreach (QPushButton* vpOldButton, mListButtonsTable)
    {
        mMapper.removeMappings(vpOldButton);
        disconnect(vpOldButton, SIGNAL(clicked()), &mMapper, SLOT(map()));
        delete vpOldButton;
    }
    mListButtonsTable.clear();
    QPushButton* vpButton;
    for (int i = 0; i < mpModel->rowCount(); ++i)
    {
        if (!mpModel->status(i).isEmpty())
        {
            vpButton = new QPushButton(TRANSLATE("восстановить"), ui->mpButtonView);
            ui->mpView->setIndexWidget(mpModel->index(i,vCount - 1), vpButton);
            ui->mpView->setRowHeight(i, 30);
            connect(vpButton, SIGNAL(clicked()), &mMapper, SLOT(map()));
            mMapper.setMapping(vpButton, i);
            mListButtonsTable.append(vpButton);
        }
    }
    ui->mpView->setColumnWidth(vCount-1, 200);

}

void ViewObjectWidget::reloadModel()
{
    ui->mpView->setModel(0);
    mpModel->load();
    ui->mpView->setModel(mpModel);
    ui->mpView->resizeRowsToContents();
    ui->mpView->resizeColumnsToContents();
    ui->mpView->setColumnWidth(2, 300);
}

void ViewObjectWidget::load(const QModelIndex &index)
{
    Q_UNUSED(index);
    reloadModel();
    setButton();
}

void ViewObjectWidget::reloadObject(int aRow)
{
    QString vStatus = mpModel->status(aRow);
    if (vStatus.isEmpty()) return;
    int vIdObject = mpModel->id(aRow);
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

QString ViewObjectWidget::name()
{
    return mpWidgetAdd->name();
}

QSize ViewObjectWidget::minSize()
{
    return QSize(0, 0);
}

QSize ViewObjectWidget::size()
{
    return QSize(1221, 508);
}

QSize ViewObjectWidget::maxSize()
{
    return QSize(0, 0);
}

void ViewObjectWidget::on_mpButtonView_clicked()
{
    if (ui->mpView->currentIndex().isValid())
    {
        on_mpView_doubleClicked(ui->mpView->currentIndex());
    }
}

QString ViewObjectWidget::idName()
{
    return mpWidgetAdd->idName();
}

void ViewObjectWidget::clickFilter()
{
    emit changeWidget(SignalWidgetType(this, mpWidgetFind));

}

void ViewObjectWidget::clickClearFilter()
{
    if (mpModel)
    {
        mpWidgetFind->clearFind();
        mpModel->clearFilter();
        reloadModel();
        ui->mpClearFilter->setVisible(false);
        ui->mpView->setProperty("find", "state1");
        emit reloadMe(this);
    }
}
