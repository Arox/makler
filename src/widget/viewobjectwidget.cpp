#include "viewobjectwidget.h"
#include "ui_viewobjectwidget.h"
#include "apartmentwidget.h"
#include "dialoguniversal.h"
#include "dialogaside.h"
#include "dialogshoot.h"
#include "phonefinddialog.h"

#include <QMessageBox>

ViewObjectWidget::ViewObjectWidget(TableModel* apModel, GeneralWidget* apWidgetAdd, QWidget *parent) :
    WidgetForControl(parent),
    ui(new Ui::ViewObjectWidget),
    mMapper(this),
    mpModel(apModel)
  ,mpWidgetAdd(apWidgetAdd)
{
    ui->setupUi(this);

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
}

ViewObjectWidget::~ViewObjectWidget()
{
    delete ui;
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
