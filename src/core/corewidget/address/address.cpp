#include "address.h"
#include "ui_address.h"

Address::Address(int aState, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Address)
  ,mAddressInCity(aState, this)
  ,mAddressInGarden(aState, this)
{
    ui->setupUi(this);
    ui->verticalLayout->insertWidget(1, &mAddressInCity);
    ui->verticalLayout->insertWidget(1, &mAddressInGarden);

    mAddressInGarden.setVisible(false);
}

Address::~Address()
{
    delete ui;
}

BaseAddressWidget &Address::data()
{
    if (mAddressInGarden.isHidden())
    {
        return mAddressInCity;
    }
    return mAddressInGarden;
}

bool Address::canSave()
{
    return mAddressInCity.canSave() || mAddressInGarden.canSave();
}

void Address::save()
{
    if (mAddressInCity.canSave()) mAddressInCity.save();
    if (mAddressInGarden.canSave()) mAddressInGarden.save();
}

void Address::load(int aId)
{
    mAddressInCity.load(aId);
    mAddressInGarden.load(aId);
}

void Address::on_mpCity_clicked()
{
    mAddressInCity.setVisible(true);
    mAddressInGarden.setVisible(false);
}

void Address::on_mpGarden_clicked()
{
    mAddressInCity.setVisible(false);
    mAddressInGarden.setVisible(true);
}

void Address::setEnabled(bool aFlag)
{
    mAddressInCity.setEnabled(aFlag);
    mAddressInGarden.setEnabled(aFlag);
}

bool Address::isEnabled() const
{
    return mAddressInCity.isEnabled() || mAddressInGarden.isEnabled();
}
