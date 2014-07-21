#include "findobjectswidget.h"
#include "ui_findobjectswidget.h"

FindObjectsWidget::FindObjectsWidget(QWidget *parent) :
    MainWidget(parent),
    ui(new Ui::FindObjectsWidget)
{
    ui->setupUi(this);

    ui->mpFind->setProperty("color", "true");
    ui->mpRemove->setProperty("remove", "true");

    ui->mpRent->setObjectName("rentbutton");
    ui->mpHome->setObjectName("homebutton");
    ui->mpApartment->setObjectName("apartmentbutton");

    ui->mpRemove->setVisible(false);

    mApartmentModel.setActive(true);
    mApartmentModel.setArchive(false);
    mApartmentModel.setAside(false);

    mRentModel.setActive(true);
    mRentModel.setArchive(false);
    mRentModel.setAside(false);

    mHomeModel.setActive(true);
    mHomeModel.setArchive(false);
    mHomeModel.setAside(false);

    mApartmentModel.setColumnChecable(0);
    mRentModel.setColumnChecable(0);
    mHomeModel.setColumnChecable(0);

    on_mpApartment_clicked();

    connect(&mApartmentModel, SIGNAL(checkStateChange()), this, SLOT(checkedView()));
    connect(&mHomeModel, SIGNAL(checkStateChange()), this, SLOT(checkedView()));
    connect(&mRentModel, SIGNAL(checkStateChange()), this, SLOT(checkedView()));
}

FindObjectsWidget::~FindObjectsWidget()
{
    delete ui;
}

void FindObjectsWidget::on_mpApartment_clicked()
{
    ui->mpRent->setChecked(false);
    ui->mpHome->setChecked(false);
    ui->mpApartment->setChecked(true);

    ui->mpView->setModel(&mApartmentModel);
    reloadView();

    if (!((TableModel*)ui->mpView->model())->isChecked())
    {
        ui->mpRemove->setVisible(false);
    }
}

void FindObjectsWidget::on_mpHome_clicked()
{
    ui->mpRent->setChecked(false);
    ui->mpApartment->setChecked(false);
    ui->mpHome->setChecked(true);

    ui->mpView->setModel(&mHomeModel);
    reloadView();
    if (!((TableModel*)ui->mpView->model())->isChecked())
    {
        ui->mpRemove->setVisible(false);
    }
}

void FindObjectsWidget::on_mpRent_clicked()
{
    ui->mpHome->setChecked(false);
    ui->mpApartment->setChecked(false);
    ui->mpRent->setChecked(true);

    ui->mpView->setModel(&mRentModel);
    reloadView();
    if (!((TableModel*)ui->mpView->model())->isChecked())
    {
        ui->mpRemove->setVisible(false);
    }
}

void FindObjectsWidget::on_mpFind_clicked()
{
    emit find(currentType());
}

void FindObjectsWidget::load(TypeObject aObject, QString aFilter)
{
    if (aFilter == " ") aFilter = "";
    ui->mpView->setModel(0);
    switch (aObject)
    {
    case APARTMENT:
        mApartmentModel.addFilter(aFilter);
        mApartmentModel.addToOrder();
        mApartmentModel.load();
        ui->mpView->setModel(&mApartmentModel);
        break;
    case HOME:
        mHomeModel.addFilter(aFilter);
        mHomeModel.addToOrder();
        mHomeModel.load();
        ui->mpView->setModel(&mHomeModel);
        break;
    case RENT:
        mRentModel.addFilter(aFilter);
        mRentModel.addToOrder();
        mRentModel.load();
        ui->mpView->setModel(&mRentModel);
        break;
    default:
        break;
    }
}

void FindObjectsWidget::reloadView()
{
    if (ui->mpView->model() == &mHomeModel)
    {
        ui->mpView->setModel(0);
        mHomeModel.load();
        ui->mpView->setModel(&mHomeModel);

    }
    else
    {
        if (ui->mpView->model() == &mRentModel)
        {
            ui->mpView->setModel(0);
            mRentModel.load();
            ui->mpView->setModel(&mRentModel);
        }
        else
        {
            ui->mpView->setModel(0);
            mApartmentModel.load();
            ui->mpView->setModel(&mApartmentModel);
        }
    }
    ui->mpView->resizeColumnsToContents();
    ui->mpView->resizeRowsToContents();
}

void FindObjectsWidget::setObjectId(int aObjectId)
{
    mApartmentModel.setObjectId(aObjectId);
    mApartmentModel.load();
    mHomeModel.setObjectId(aObjectId);
    mHomeModel.load();
    mRentModel.setObjectId(aObjectId);
    mRentModel.load();
    reloadView();
}

void FindObjectsWidget::checkedView()
{
    bool vVisible = ((TableModel*)ui->mpView->model())->isChecked();
    ui->mpRemove->setVisible(vVisible);
}

void FindObjectsWidget::on_mpRemove_clicked()
{
    ((FindTableModel*)ui->mpView->model())->removeChecked();
    reloadView();
}

void FindObjectsWidget::on_mpClear_clicked()
{
    if (dynamic_cast<TableModelApartmentClient*>(ui->mpView->model()))
    {
        mApartmentModel.clear();
    }
    if (dynamic_cast<TableModelHomeClient*>(ui->mpView->model()))
    {
        mHomeModel.clear();
    }
    if (dynamic_cast<TableModelRentClient*>(ui->mpView->model()))
    {
        mRentModel.clear();
    }
    reloadView();
}

FindObjectsWidget::TypeObject FindObjectsWidget::currentType()
{
    if (ui->mpView->model() == &mApartmentModel)
    {
        return APARTMENT;
    }
    if (ui->mpView->model() == &mRentModel)
    {
        return RENT;
    }
    if (ui->mpView->model() == &mHomeModel)
    {
        return HOME;
    }
    return UNKNOW;
}

void FindObjectsWidget::on_mpButtonView_clicked()
{
    if (ui->mpView->currentIndex().isValid())
    {
        on_mpView_doubleClicked(ui->mpView->currentIndex());
    }
}

void FindObjectsWidget::on_mpView_doubleClicked(const QModelIndex &index)
{
    int id = 0;
    TypeObject vType = currentType();
    switch (vType) {
    case APARTMENT:
        id = mApartmentModel.id(index.row());
        break;
    case RENT:
        id = mRentModel.id(index.row());
        break;
    case HOME:
        id = mHomeModel.id(index.row());
        break;
    default:
        break;
    }

    emit activateObject(vType, id);
}
