#include "viewobjects.h"
#include "ui_viewobjects.h"
#include "language.h"

ViewObjects::ViewObjects(QWidget *parent) :
    MainWidget(parent),
    ui(new Ui::ViewObjects)
{
    ui->setupUi(this);

    mModelsList << &mApartmnet << &mHome << &mRent << &mClient;

    foreach(TableModel* vpModel, mModelsList)
    {
        vpModel->setButtonStatus(false);
        vpModel->setColumnChecable(0);
        vpModel->setColumnAgent(vpModel->columnCount(QModelIndex()));
        connect(vpModel, SIGNAL(checkStateChange()), this, SLOT(selectChange()));
    }

    ui->mpActive->setChecked(true);
    on_mpActive_clicked();
    on_mpApartment_clicked();

    loadAgents();
}

ViewObjects::~ViewObjects()
{
    delete ui;
}

void ViewObjects::load(ViewObjects::TypeObject aObject, QString aFilter)
{
    TableModel* vpModel = 0;
    mOldFilter = aFilter;
    switch(aObject)
    {
    case APARTMENT:
        vpModel = &mApartmnet;
        break;
    case RENT:
        vpModel = &mRent;
        break;
    case HOME:
        vpModel = &mHome;
        break;
    case CLIENT:
        vpModel = &mClient;
        break;
    default:
        break;
    }
    if (vpModel)
    {
        if (ui->mpAgents->itemData(ui->mpAgents->currentIndex()).toInt() > 0)
        {
            if (aFilter.length())
            {
                aFilter = QString("%1 AND agent_fk = %2").arg(aFilter).arg(ui->mpAgents->itemData(ui->mpAgents->currentIndex()).toInt());
            }
            else
            {
                aFilter = QString("agent_fk = %1").arg(ui->mpAgents->itemData(ui->mpAgents->currentIndex()).toInt());
            }
        }
        ui->mpView->setModel(0);
        vpModel->addFilter(aFilter);
        vpModel->load();
        vpModel->clearSelection();
        ui->mpView->setModel(vpModel);
        ui->mpView->resizeRowsToContents();
        ui->mpView->resizeColumnsToContents();
        ui->mpView->horizontalHeader()->setStretchLastSection(true);
    }
}

ViewObjects::TypeObject ViewObjects::type()
{
    if (ui->mpView->model() == &mApartmnet) return APARTMENT;
    if (ui->mpView->model() == &mHome) return HOME;
    if (ui->mpView->model() == &mRent) return RENT;
    if (ui->mpView->model() == &mClient) return CLIENT;
    return UNDEFINED;
}

void ViewObjects::setCheckStatus(QPushButton* apButton)
{
    ui->mpActive->setChecked(false);
    ui->mpArchive->setChecked(false);
    ui->mpAside->setChecked(false);

    apButton->setChecked(true);

    foreach (TableModel* vpModel, mModelsList)
    {
        vpModel->setActive(ui->mpActive->isChecked());
        vpModel->setArchive(ui->mpArchive->isChecked());
        vpModel->setAside(ui->mpAside->isChecked());
    }

    TableModel* vpModel =dynamic_cast<TableModel*>(ui->mpView->model());
    if (vpModel)
    {
        ui->mpView->setModel(0);
        vpModel->load();
        ui->mpView->setModel(vpModel);
    }
}

void ViewObjects::on_mpActive_clicked()
{
    setCheckStatus(ui->mpActive);
}

void ViewObjects::on_mpAside_clicked()
{
    setCheckStatus(ui->mpAside);
}

void ViewObjects::on_mpArchive_clicked()
{
    setCheckStatus(ui->mpArchive);
}

void ViewObjects::on_mpApartment_clicked()
{
    ui->mpApartment->setChecked(true);
    ui->mpRent->setChecked(false);
    ui->mpHome->setChecked(false);
    ui->mpClient->setChecked(false);

    load(APARTMENT, "");

    emit changeModel(APARTMENT);
    selectChange();
}

void ViewObjects::on_mpRent_clicked()
{
    ui->mpApartment->setChecked(false);
    ui->mpRent->setChecked(true);
    ui->mpHome->setChecked(false);
    ui->mpClient->setChecked(false);

    load(RENT, "");

    emit changeModel(RENT);
    selectChange();
}

void ViewObjects::on_mpHome_clicked()
{
    ui->mpApartment->setChecked(false);
    ui->mpRent->setChecked(false);
    ui->mpHome->setChecked(true);
    ui->mpClient->setChecked(false);

    load(HOME, "");

    emit changeModel(HOME);
    selectChange();
}

void ViewObjects::on_mpClient_clicked()
{
    ui->mpApartment->setChecked(false);
    ui->mpRent->setChecked(false);
    ui->mpHome->setChecked(false);
    ui->mpClient->setChecked(true);

    load(CLIENT, "");

    emit changeModel(CLIENT);
    selectChange();
}

void ViewObjects::changeAgent(int aIdAgent)
{
    TableModel* vpModel = dynamic_cast<TableModel*>(ui->mpView->model());
    if (!vpModel) return;
    foreach (int vIdObject, vpModel->checkedId())
    {
        execQuery(QString("UPDATE objects SET agent_fk = %1 WHERE id = %2")
                  .arg(aIdAgent)
                  .arg(vIdObject));
    }
    load(type(), mOldFilter);
}

void ViewObjects::selectChange()
{
    TableModel* vpModel = dynamic_cast<TableModel*>(ui->mpView->model());
    if (!vpModel) return;
    if (vpModel->checkedId().count() == vpModel->rowCount())
    {
        ui->mpSelectAll->setText(TRANSLATE("Снять выделение"));
    }
    else
    {
        ui->mpSelectAll->setText(TRANSLATE("Выделить все"));
    }
    emit isSelect(vpModel->isChecked());
}

bool ViewObjects::isSelected()
{
    TableModel* vpModel = dynamic_cast<TableModel*>(ui->mpView->model());
    if (!vpModel) return false;
    return vpModel->isChecked();
}

void ViewObjects::removeSelected()
{
    TableModel* vpModel = dynamic_cast<TableModel*>(ui->mpView->model());
    if (!vpModel) return;
    foreach(int vId, vpModel->checkedId())
    {
        vpModel->remove(vId);
    }

}

void ViewObjects::on_mpSelectAll_clicked()
{
    TableModel* vpModel = dynamic_cast<TableModel*>(ui->mpView->model());
    if (!vpModel) return;
    QVariant vValue = (vpModel->checkedId().count() == vpModel->rowCount() ? Qt::Unchecked : Qt::Checked);
    for (int i = 0; i < vpModel->rowCount(); ++i)
    {
        vpModel->setData(vpModel->index(i, 0), vValue, Qt::CheckStateRole);
    }
    ui->mpView->reset();
}

void ViewObjects::loadAgents()
{
    ui->mpAgents->clear();
    ResponseType vResponse = execQuery(QString("SELECT mans.id as id, mans.sername as sername, mans.name as name, mans.patronymic as patronymic FROM users INNER JOIN mans ON users.man_fk = mans.id WHERE users.is_active = TRUE ORDER BY sername, name, patronymic"));
    ui->mpAgents->addItem(TRANSLATE("Все"), -1);
    for (int i = 0; i < vResponse.count(); ++i)
    {
        ResponseRecordType vRecord = vResponse[i];
        QString vName = QString("%1 %2 %3")
                .arg(vRecord["sername"].toString())
                .arg(vRecord["name"].toString())
                .arg(vRecord["patronymic"].toString());
        int vId = vRecord["id"].toInt();
        ui->mpAgents->addItem(vName, vId);
    }
}

void ViewObjects::on_mpAgents_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    load(type(), mOldFilter);
}

void ViewObjects::reload()
{
    load(type(), mOldFilter);
}
